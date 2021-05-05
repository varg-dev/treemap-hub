
#include <measurements/metrics/RelativeDirectionChange.h>


#include <limits>

#include <glm/common.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>


#include <treemap/layout/Rect.h>


namespace
{


inline std::size_t getIndex(unsigned int x, unsigned int y, std::size_t width)
{
    return y * width + x;
}

inline std::pair<unsigned int, unsigned int> getComponents(std::size_t index, std::size_t width)
{
    return std::make_pair(index % width, index / width);
}


std::pair<float,float> compareMatrices(std::vector<float> & matrix, std::vector<float> & matrixComp, std::size_t sharedNodeCount)
{
    if (matrix.size() <= 1)
    {
        return std::make_pair(0.0f, 0.0f);
    }

    std::vector<float> rowDeltas(sharedNodeCount-1);

    auto squaredDeltaSum = 0.0f;
    auto squaredDeltaSumForAAD = 0.0f;
    for (size_t i = 0; i < sharedNodeCount; ++i)
    {
        auto sum = 0.0f;

        auto indexOffset = 0;
        for (size_t j = 0; j < sharedNodeCount; ++j)
        {
            if (i == j)
            {
                indexOffset = -1;

                continue;
            }
            else
            {
                const auto index = getIndex(i, j, sharedNodeCount);

                const auto element = matrix.at(index);
                const auto elementComp = matrixComp.at(index);

                auto angleDelta = element - elementComp;
                while (angleDelta > glm::pi<float>())
                {
                    angleDelta -= 2 * glm::pi<float>();
                }
                while (angleDelta < - glm::pi<float>())
                {
                    angleDelta += 2 * glm::pi<float>();
                }

                rowDeltas[j + indexOffset] = angleDelta;
                sum += angleDelta;
            }
        }

        auto average = sum / rowDeltas.size();
        for (size_t j = 0; j < rowDeltas.size(); ++j)
        {
            squaredDeltaSum += glm::pow(rowDeltas[j] - average, 2.0f);
            squaredDeltaSumForAAD += glm::pow(rowDeltas[j], 2.0f);
        }
    }

    return std::make_pair(glm::pow(squaredDeltaSum / (sharedNodeCount * (sharedNodeCount-1)), 0.5f),
                          glm::pow(squaredDeltaSumForAAD / (sharedNodeCount * (sharedNodeCount-1)), 0.5f));
}

void fillMatrix(std::vector<float> & matrix, const LinearizedTree *tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const LinearizedBuffer<Rect> &layout, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    for (auto index = size_t(0); index < matrix.size(); ++index) {
        auto x = 0u;
        auto y = 0u;

        std::tie(x, y) = getComponents(index, occuringInBothRevisions.size());

        if (x == y)
        {
            matrix[index] = 0.0f;

            return;
        }

        const auto a = layout[tree->at(nodeIndexById.at(occuringInBothRevisions[x]))].center();
        const auto b = layout[tree->at(nodeIndexById.at(occuringInBothRevisions[y]))].center();

        if (glm::dot(a, b) <= glm::epsilon<float>())
        {
            matrix[index] = 0.0f;
        }
        else
        {
            matrix[index] = glm::atan(b.y - a.y, b.x - a.x);
        }
    }
}


std::pair<float,float> computeAngularMetrics(LinearizedTree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const LinearizedBuffer<Rect> & layout, LinearizedTree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const LinearizedBuffer<Rect> & layoutComp, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    const auto sharedNodeCount = occuringInBothRevisions.size();

    if (sharedNodeCount <= 1)
    {
        return std::make_pair(0.0f, 0.0f);
    }

    // Initialize angularOrientations
    std::vector<float> angularOrientations(sharedNodeCount * sharedNodeCount);
    std::vector<float> angularOrientationsComp(sharedNodeCount * sharedNodeCount);

    fillMatrix(angularOrientations, tree, nodeIndexById, layout, occuringInBothRevisions);
    fillMatrix(angularOrientationsComp, treeComp, nodeIndexByIdComp, layoutComp, occuringInBothRevisions);

    float rdc = 0;
    float aad = 0;

    std::tie(rdc, aad) = compareMatrices(angularOrientations, angularOrientationsComp, sharedNodeCount);

    rdc /= glm::pi<float>();
    aad /= glm::pi<float>();

    return std::make_pair(rdc, aad);
}


} // namespace


RelativeDirectionChange::RelativeDirectionChange()
{
}


float RelativeDirectionChange::compute(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    return computeAverage(trees, layouts, maxId, index2ids, nodeIndexByIds);
}


float RelativeDirectionChange::computeAverage(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    if (trees.size() < 2)
    {
        return 0.0f;
    }

    auto result = 0.0f;
    for (auto i = 1ull; i < trees.size(); ++i)
    {
        std::vector<std::uint32_t> occuringInBothRevisions;

        const auto currentTree = trees[i];
        const auto & currentIDs = index2ids[i];
        const auto prevTree = trees[i-1];
        const auto & prevNodeIndicesById = nodeIndexByIds[i-1];

        for (size_t j = 0; j < currentTree->size(); j++)
        {
            const auto node = currentTree->at(j);
            const auto currentId = currentIDs.at(node);
            const auto & layoutElement = layouts[i][node];

            if (layoutElement.area() >= glm::epsilon<float>())
            {
                const auto it = prevNodeIndicesById.find(currentId);

                if (it != prevNodeIndicesById.end())
                {
                    const auto compNode = prevTree->at(it->second);

                    if (compNode != nullptr)
                    {
                        const auto & layoutElementComp = layouts[i-1][compNode];

                        if (layoutElementComp.area() >= glm::epsilon<float>())
                        {
                            occuringInBothRevisions.push_back(currentId);
                        }
                    }
                }
            }
        }

        result += relativeDirectionChange(prevTree, nodeIndexByIds[i-1], layouts[i-1], currentTree, nodeIndexByIds[i], layouts[i], occuringInBothRevisions);
    }

    return result / (trees.size() - 1);
}


float RelativeDirectionChange::relativeDirectionChange(LinearizedTree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const LinearizedBuffer<Rect> & layout,
                          LinearizedTree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const LinearizedBuffer<Rect> & layoutComp,
                          const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    auto result = 0.0f;
    auto other = 0.0f;

    std::tie(result, other) = computeAngularMetrics(tree, nodeIndexById, layout, treeComp, nodeIndexByIdComp, layoutComp, occuringInBothRevisions);

    return result;
}
