
#include <treemap-measurements/metrics/VisualChange.h>


#include <glm/gtc/constants.hpp>

#include <treemap-layouting/layout/Rect.h>


VisualChange::VisualChange()
{
}


float VisualChange::compute(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    return computeAverage(trees, layouts, maxId, index2ids, nodeIndexByIds);
}


float VisualChange::computeAverage(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
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

        result += visualChange(prevTree, nodeIndexByIds[i-1], layouts[i-1], currentTree, nodeIndexByIds[i], layouts[i], occuringInBothRevisions);
    }

    return result / (trees.size() - 1);
}


float VisualChange::visualChange(const LinearizedTree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const LinearizedBuffer<Rect> & layout,
                                 LinearizedTree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const LinearizedBuffer<Rect> & layoutComp,
                                 const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 1)
    {
        return 0.0f;
    }

    auto summedVisualChange = 0.0f;

    for (const auto & id : occuringInBothRevisions)
    {
        const auto node = tree->at(nodeIndexById.at(id));
        const auto nodeComp = treeComp->at(nodeIndexByIdComp.at(id));

        const auto & rect = layout.at(node);
        const auto & rectComp = layoutComp.at(nodeComp);

        const auto intersection = rect.intersection(rectComp);

        summedVisualChange += rect.area() + rectComp.area() - 2.0f * intersection.area();
    }

    return summedVisualChange / numberOfNodes;
}
