
#include <measurements/metrics/AspectRatio.h>


#include <limits>
#include <numeric>

#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>

#include <treemap/layout/Rect.h>


AspectRatio::AspectRatio()
{
}


float AspectRatio::compute(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    return computeAverage(trees, layouts, maxId, index2ids, nodeIndexByIds);
}


float AspectRatio::computeAverage(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    if (trees.size() < 1)
    {
        return 0.0f;
    }

    auto result = 0.0f;
    for (auto i = 0ull; i < trees.size(); ++i)
    {
        const auto aspectRatios = this->aspectRatios(trees[i], layouts[i]);

        result += aspectRatios.size() > 0 ? std::accumulate(aspectRatios.begin(), aspectRatios.end(), 0.0f) / aspectRatios.size() : 1.0f;
    }

    return result / trees.size();
}


float AspectRatio::computeChange(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    return computeChangeAverage(trees, layouts, maxId, index2ids, nodeIndexByIds);
}


float AspectRatio::computeChangeAverage(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
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

        result += aspectRatioChange(prevTree, nodeIndexByIds[i-1], layouts[i-1], currentTree, nodeIndexByIds[i], layouts[i], occuringInBothRevisions);
    }

    return result / (trees.size() - 1);
}


LinearizedBuffer<float> AspectRatio::aspectRatios(const LinearizedTree * tree, const RectangularTreemapLayout & layout)
{
    LinearizedBuffer<float> result(std::vector<float>(tree->size()));

    tree->nodesDo([& layout, & result](const LinearizedTreeNode * node) {
        const auto & rect = layout.at(node->index());

        result[node] = rect.area() >= std::numeric_limits<float>::epsilon() ? rect.aspectRatio() : 0.0f;
    });

    return result;
}

float AspectRatio::aspectRatioChange(LinearizedTree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const LinearizedBuffer<Rect> & layout, LinearizedTree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const LinearizedBuffer<Rect> & layoutComp, const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 1)
    {
        return 1.0f;
    }

    auto summedAspectRatioChange = 0.0f;

    for (const auto & id : occuringInBothRevisions)
    {
        const auto & rect = layout.at(tree->at(nodeIndexById.at(id)));
        const auto & rectComp = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id)));

        const auto ar = rect.aspectRatio();
        const auto arComp = rectComp.aspectRatio();

        summedAspectRatioChange += glm::max(ar / arComp, arComp / ar);
    }

    return summedAspectRatioChange / numberOfNodes;
}
