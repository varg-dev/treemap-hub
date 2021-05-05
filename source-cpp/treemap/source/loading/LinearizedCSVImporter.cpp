
#include <treemap/loading/LinearizedCSVImporter.h>

#include <cassert>
#include <cstdlib>

#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include <fstream>
#include <functional>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


namespace
{


inline float to_float(const std::string & value)
{
    if (value.empty())
    {
        return 0.0f;
    }

    return std::stof(value);
}

inline float to_float(const std::string_view & value)
{
    if (value.empty())
    {
        return 0.0f;
    }

    return std::strtof(value.begin(), nullptr);
}

std::vector<std::pair<int,std::string_view>> extractLines(char* begin, char* end)
{
    assert(begin <= end);

    std::vector<std::pair<int,std::string_view>> result;
    result.reserve(4500000); // we know our datasets...

    char * current = begin;
    while (current < end)
    {
        const auto next = std::find(current, end, '\n');

        result.emplace_back(0, std::string_view(current, next-current));
        current = next+1;
    }

    return result;
}

template <typename Callback>
inline void parseLine(const std::string_view & line, const std::string::size_type start_pos, const char delimiter, Callback && callback)
{
    if (start_pos == std::string::npos)
    {
        // No rest of line
        return;
    }

    int i = 0;
    auto last_pos = start_pos;

    // bool hasEscapeCharacter = line.find(escapeCharacter, last_pos);

    for (auto current_pos = line.find(delimiter, last_pos);
         current_pos != std::string::npos;
         current_pos = line.find(delimiter, last_pos))
    {
        // no handling of escaped strings
        callback(line.substr(last_pos, current_pos - last_pos), i);

        last_pos = current_pos + 1;

        ++i;
    }

    callback(line.substr(last_pos), i);
}

template <typename Callback>
inline void parseLine(const std::string_view & line, const std::string::size_type start_pos, const char delimiter, const std::vector<int> & filter, Callback && callback)
{
    if (start_pos == std::string::npos)
    {
        // No rest of line
        return;
    }

    int i = 0;
    auto last_pos = start_pos;
    auto nextMatch = filter.begin();

    for (auto current_pos = line.find(delimiter, last_pos);
         current_pos != std::string::npos && nextMatch != filter.end();
         current_pos = line.find(delimiter, last_pos))
    {
        // no handling of escaped strings
        if (i == *nextMatch)
        {
            callback(line.substr(last_pos, current_pos - last_pos), i);
            ++nextMatch;
        }

        last_pos = current_pos + 1;
        ++i;
    }

    if (nextMatch != filter.end() && i == *nextMatch)
    {
        callback(line.substr(last_pos), i);
    }
}

inline void parseHeader(const std::string_view & line, const std::string & weightAttribute, int & weightIndex)
{
    const auto end_of_name = line.find(';');

    //PARSE HEADER
    parseLine(line, end_of_name+1, ';', [& weightAttribute, & weightIndex](const std::string_view & attribute_name, const int i) {
        const auto isNameAttribute = attribute_name.find("name") != std::string::npos || attribute_name.find("Name") != std::string::npos;

        if (isNameAttribute)
        {
            return; // Couldn't handle string attributes for now
        }

        if (attribute_name == weightAttribute)
        {
            weightIndex = i;
        }
    });
}


} // namespace


LinearizedCSVImporter::LinearizedCSVImporter()
: LinearizedCSVImporter(false)
{
}

LinearizedCSVImporter::LinearizedCSVImporter(bool enableVerboseOutput)
: m_enableVerboseOutput(enableVerboseOutput)
{
}

LinearizedTree * LinearizedCSVImporter::load(const std::string& filename, const std::string & weightAttribute, std::vector<std::string>* paths)
{
    std::ifstream file(filename, std::ios::in);

    if (!file.good())
    {
        std::cerr << "[tree-data loading] unable to read file " << filename << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    const auto contents_length = file.tellg();

    if (contents_length <= 0)
    {
        std::cerr << "[tree-data loading] no contents in " << filename << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::beg);
    auto contents = std::vector<char>(contents_length);
    file.read(contents.data(), contents_length);
    const auto end_of_header = std::find(contents.begin(), contents.end(), '\n');
    const auto header = std::string_view(contents.data(), end_of_header - contents.begin());

    int weightIndex = -1;
    parseHeader(header, weightAttribute, weightIndex);

    std::vector<std::pair<int, std::string_view>> level = extractLines(contents.data() + (end_of_header - contents.begin() + 1), contents.data()+contents_length);

    if (level.size() < 1)
    {
        return nullptr;
    }

    // final data
    std::vector<LinearizedTreeNode> nodes;
    std::vector<std::string_view> values;
    if(paths)
    {
        paths->clear();
    }

    // Guess
    nodes.reserve(level.size() * 2);
    values.reserve(level.size() * 2);
    if(paths)
    {
        paths->reserve(level.size()*2);
    }

    // Initialize root
    nodes.emplace_back(0, 0, -1);
    values.push_back("");
    if(paths)
    {
        paths->emplace_back("");
    }

    const auto numberOfLeaves = level.size();

    std::vector<std::pair<std::uint32_t, std::uint32_t>> slices;

    slices.emplace_back(0, 0);

    while(!level.empty())
    {
        int nextLevelEnd = 0;
        int sliceStart = nodes.size();

        // loop over all parents in this level
        for(int pstart = 0; pstart<level.size(); )
        {
            const auto nextLevelBegin = nextLevelEnd;
            const auto parentIdx = level[pstart].first;
            auto pend = pstart+1; // exclusive end
            while (pend < level.size() && level[pend].first == parentIdx) // TODO: use binary search?
            {
                pend++;
            }

            // loop over all descendants of this parent
            std::unordered_map<std::string_view, int> childIds; // only used for non-leaf children
            // std::map<std::string_view, int> childIds; // only used for non-leaf children; unordered_map is faster
            for (auto c = pstart; c != pend; ++c)
            {
                const auto line = level[c].second;
                const auto it = std::find(line.begin(), line.end(), '/');
                auto & parentNode = nodes[parentIdx];
                // leaf node found
                if (it==line.end())
                {
                    parentNode.numberOfChildren()++;

                    if (parentNode.firstChild() == LinearizedTree::invalidIndex)
                    {
                        parentNode.setFirstChild(nodes.size());
                    }

                    const auto sep = line.find(';');
                    assert(sep != std::string::npos);

                    nodes.emplace_back(nodes.size(), parentNode.depth()+1, parentIdx);
                    values.push_back(line.substr(sep+1));
                    if(paths) paths->push_back(paths->at(parentIdx) + '/' + std::string(line.substr(0, sep)));

                    continue;
                }

                const auto dirname = std::string_view(line.begin(), it-line.begin());

                auto parentIt = childIds.find(dirname);
                if (parentIt == childIds.end())
                { // never seen parent found
                    parentNode.numberOfChildren()++;

                    parentIt = childIds.emplace(dirname, nodes.size()).first;
                    if (parentNode.firstChild() == LinearizedTree::invalidIndex)
                    {
                        parentNode.setFirstChild(nodes.size());
                    }

                    nodes.emplace_back(nodes.size(), parentNode.depth()+1, parentIdx);
                    values.emplace_back("");
                    if(paths) paths->push_back(paths->at(parentIdx) + '/' + std::string(dirname));
                }

                auto restline = std::string_view(it+1, line.end()-it-1);
                level[nextLevelEnd++] = { parentIt->second, restline };
            } // end children

            if (true)
            {
                std::sort(level.begin() + nextLevelBegin, level.begin() + nextLevelEnd,
                    [](auto p1, auto p2) { return p1.first < p2.first; });
            }
            else
            {
                std::stable_sort(level.begin() + nextLevelBegin, level.begin() + nextLevelEnd,
                    [](auto p1, auto p2) { return p1.first < p2.first; });
            }

            pstart = pend;
        } // end parent

        level.erase(level.begin() + nextLevelEnd, level.end());

        slices.emplace_back(sliceStart, nodes.size() - 1);
    } // end bfs

    auto tree = new LinearizedTree();

    tree->setNumberOfLeaves(numberOfLeaves);

    tree->addAttribute(weightAttribute);

    tree->slices() = slices;
    tree->nodes().setVector(std::move(nodes));
    auto & weightValues = tree->attributeValues(weightAttribute);
    weightValues.resize(tree->size());

    auto filter = std::vector<int>({ weightIndex });
    std::sort(filter.begin(), filter.end());

    const auto perNodeAttributeParsing = [& tree, &values, & weightValues, & weightIndex, & filter](const LinearizedTreeNode * node) {
        parseLine(values[node->index()], 0, ';', filter, [& node, & weightValues, & weightIndex](const std::string_view & attribute_value, const int k) {
            /*if (k != weightIndex)
            {
                return;
            }*/

            const auto value = to_float(attribute_value);

            if (k == weightIndex)
            {
                weightValues[node] = value;
            }
        });
    };

    tree->leavesDo(perNodeAttributeParsing);

    return tree;
}
