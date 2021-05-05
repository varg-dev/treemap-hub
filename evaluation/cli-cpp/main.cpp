
#include <iostream>
#include <sstream>
#include <numeric>
#include <fstream>


#include <cppassist/cmdline/ArgumentParser.h>

#include "common.h"

#include "export/export.h"
#include <treemap/loading/LinearizedCSVImporter.h>
#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/preprocessing/AttributeSumAggregation.h>
#include <treemap/preprocessing/AggregationType.h>
#include <treemap/preprocessing/TreeProcessing.h>
#include <treemap/layouting/TreemapLayouter.h>
#include <treemap/layout/Rect.h>


namespace
{


void streamUsage(std::ostream & stream)
{
    stream.flush();
}


} // namespace


int main(int argc, char ** argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::clog.tie(nullptr);
    std::cerr.tie(nullptr);

    auto arguments = cppassist::ArgumentParser();
    arguments.parse(argc, argv);

    const auto multipleDatasets = arguments.isSet("--");

    auto filename = arguments.value<std::string>("--dataset", "");
    auto filenames = arguments.value<std::string>("--datasets", "");
    auto filenamesPrefix = arguments.value<std::string>("--datasets-prefix", "");
    auto weightAttribute = arguments.value<std::string>("--weights", "");
    auto layoutAlg = arguments.value<std::string>("--algorithm", "hilbert");

    if (weightAttribute == "" || (!multipleDatasets && filename == "" && filenames == ""))
    {
        std::cerr <<
            "No dataset found or no weights attribute passed" <<
            std::endl;
        return 1;
    }

    // const auto flatImport = arguments.isSet("-f");
    const auto exportSVG = arguments.isSet("-es");
    const auto exportLayout = arguments.isSet("-el");
    const auto printStatistics = arguments.isSet("-p");

    streamUsage(std::cerr);

    auto linearizedImporter = LinearizedCSVImporter();
    auto treeProcessing = TreeProcessing();
    auto aggregation = AttributeSumAggregation();
    auto layouter = TreemapLayouter();

    if (layoutAlg == "hilbert") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_Greedy);
    } else if (layoutAlg == "moore") {
        layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_Greedy);
    } else {
        std::cerr << "Unknown algorithm: " << layoutAlg << std::endl;
    }

    std::vector<LinearizedTree *> linearizedTrees;
    std::vector<LinearizedBuffer<std::uint32_t>> index2id; // tree index to global id
    std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> id2index; // global id to tree index
    std::unordered_map<std::string, std::uint32_t> path2id; // path to global id

    auto mapToGlobalIds = [&path2id](const std::vector<std::string>& paths) {
        LinearizedBuffer<std::uint32_t> res(std::vector<std::uint32_t>(paths.size()));

        for (int index = 0; index < paths.size(); ++index)
        {
            auto it = path2id.find(paths[index]);
            if(it == end(path2id))
            {
                it = path2id.emplace(paths[index], path2id.size()).first;
            }

            res[index] = it->second;
        }

        return res;
    };

    if (filenames != "")
    {
        std::string f;
        std::ifstream fs(filenames);
        while (fs.good() && std::getline(fs, f))
        {
            LinearizedTree * linearizedTree = nullptr;

            std::vector<std::string> paths;
            linearizedTree = treeProcessing.removeZeroWeightNodesAndDeleteSource(linearizedImporter.load(filenamesPrefix + f, weightAttribute, &paths), weightAttribute, &paths);

            if (linearizedTree == nullptr) // ignore empty trees
            {
                continue;
            }

            auto & currentIndex2id = index2id.emplace_back(mapToGlobalIds(paths));
            auto & currentID2index = id2index.emplace_back(std::unordered_map<std::uint32_t, std::uint32_t>({}));

            for (auto i = 0; i < currentIndex2id.size(); ++i)
            {
                currentID2index[currentIndex2id[i]] = i;
            }

            linearizedTrees.push_back(linearizedTree);
        }

        // TODO sort later trees by global_id ?
    }

    if (multipleDatasets)
    {
        std::string f;
        while (std::getline(std::cin, f))
        {
            LinearizedTree * linearizedTree = nullptr;

            std::vector<std::string> paths;
            linearizedTree = treeProcessing.removeZeroWeightNodesAndDeleteSource(linearizedImporter.load(f, weightAttribute, &paths), weightAttribute, &paths);

            if (linearizedTree == nullptr) // ignore empty trees
            {
                continue;
            }

            auto & currentIndex2id = index2id.emplace_back(mapToGlobalIds(paths));
            auto & currentID2index = id2index.emplace_back(std::unordered_map<std::uint32_t, std::uint32_t>({}));

            for (auto i = 0; i < currentIndex2id.size(); ++i)
            {
                currentID2index[currentIndex2id[i]] = i;
            }

            linearizedTrees.push_back(linearizedTree);
        }

        // TODO sort later trees by global_id ?
    }

    if (filename != "")
    {
        LinearizedTree * linearizedTree = nullptr;

        std::vector<std::string> paths;
        linearizedTree = treeProcessing.removeZeroWeightNodesAndDeleteSource(linearizedImporter.load(filename, weightAttribute, &paths), weightAttribute, &paths);
        index2id.emplace_back(mapToGlobalIds(paths));

        linearizedTrees.push_back(linearizedTree);
    }

    /*if (flatImport)
    {
        for (auto & linearizedTree : linearizedTrees)
        {
            linearizedTree = treeProcessing.flattenAndDeleteSource(linearizedTree, weightAttribute);
        }
    }*/

    if (printStatistics)
    {
        std::cerr << "Dataset " << filename << (multipleDatasets ? " (multiple)" : "") << " weights attribute " << weightAttribute << std::endl;
    }

    std::vector<LinearizedBuffer<Rect>> layouts;

    LinearizedBuffer<float> * weightValues = nullptr; // last weight values
    
    auto i = 0;
    for (const auto & linearizedTree : linearizedTrees)
    {
        ++i;
        weightValues = &linearizedTree->attributeValues(weightAttribute);

        if (printStatistics)
        {
            linearizedTree->outputStatistics(std::cout);
        }

        aggregation.process(linearizedTree, *weightValues);

        LinearizedBuffer<Rect> layout(std::vector<Rect>(linearizedTree->size()));

        layouter.process(
            linearizedTree,
            *weightValues,
            layout
        );

        layouts.push_back(layout);
    }

    // Continue with last layout and tree

    const auto linearizedTree = linearizedTrees.back();
    const auto & layout = layouts.back();

    if (exportSVG)
    {
        writeToSVG(layout, linearizedTree, 1000.0f, false, SVGColorType::GrayScaleContinuous, std::cout);
    }
    if (exportLayout)
    {
        writeToFile(layout, std::cout);
    }

    for (const auto & linearizedTree : linearizedTrees)
    {
        delete linearizedTree;
    }

    return 0;
}
