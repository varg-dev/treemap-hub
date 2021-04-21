
#include <treemap-measurements/metrics/RelativePositionChange.h>


#include <glm/gtc/constants.hpp>

#include <treemap-layouting/layout/Rect.h>


namespace
{


Rect getSection(const Rect & r1, size_t i)
{
    // divide the space around r1 into 8 sections s1 to s8
    //              left,             bottom,             width,      height
    switch (i) {
    case 0:
        return Rect(r1.left() - 1.0f, r1.bottom(),        1.0f,       r1.height()); // left
    case 1:
        return Rect(r1.left(),        r1.top(),           r1.width(), 1.0f);        // top
    case 2:
        return Rect(r1.right(),       r1.bottom(),        1.0f,       r1.height()); // right
    case 3:
        return Rect(r1.left(),        r1.bottom() - 1.0f, r1.width(), 1.0f);        // bottom
    case 4:
        return Rect(r1.left() - 1.0f, r1.top(),           1.0f,       1.0f);        // top-left
    case 5:
        return Rect(r1.right(),       r1.top(),           1.0f,       1.0f);        // top-right
    case 6:
        return Rect(r1.right(),       r1.bottom() - 1.0f, 1.0f,       1.0f);        // bottom-right
    default:
    case 7:
        return Rect(r1.left() - 1.0f, r1.bottom() - 1.0f, 1.0f,       1.0f);        // bottom-left
    }
}


} // namespace


RelativePositionChange::RelativePositionChange()
{
}


float RelativePositionChange::compute(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    return computeAverage(trees, layouts, maxId, index2ids, nodeIndexByIds);
}


float RelativePositionChange::computeAverage(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
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

        result += relativePositionChange(prevTree, nodeIndexByIds[i-1], layouts[i-1], currentTree, nodeIndexByIds[i], layouts[i], occuringInBothRevisions);
    }

    return result / (trees.size() - 1);
}


float RelativePositionChange::relativePositionChange(const LinearizedTree * tree, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexById, const LinearizedBuffer<Rect> & layout,
                                                     const LinearizedTree * treeComp, const std::unordered_map<std::uint32_t, std::uint32_t> & nodeIndexByIdComp, const LinearizedBuffer<Rect> & layoutComp,
                                                     const std::vector<std::uint32_t> & occuringInBothRevisions)
{
    // implemented according to "Stable Treemaps via Local Moves" Sondag et al.

    auto numberOfNodes = occuringInBothRevisions.size();

    if (numberOfNodes < 1)
    {
        return 0.0f;
    }

    auto summedPositionChange = 0.0f;

    for (const auto & id1 : occuringInBothRevisions)
    {
        for (const auto & id2 : occuringInBothRevisions)
        {
            if (tree->at(nodeIndexById.at(id1))->parent() != tree->at(nodeIndexById.at(id2))->parent())
            {
                // do not compare rectangles from different parents
                // (although this shall not happen as IDs are given by path)
                continue;
            }
            const auto & rect1 = layout.at(tree->at(nodeIndexById.at(id1)));
            const auto & rectComp1 = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id1)));

            const auto & rect2 = layout.at(tree->at(nodeIndexById.at(id2)));
            const auto & rectComp2 = layoutComp.at(treeComp->at(nodeIndexByIdComp.at(id2)));

            float change = 0.0f;
            for (size_t i = 0; i < 8; i++)
            {
                const auto intersection = getSection(rect1, i).intersection(rect2);
                const auto intersectionComp = getSection(rectComp1, i).intersection(rectComp2);
                const auto newChange = std::abs(intersection.area() / rect2.area() - intersectionComp.area() / rectComp2.area());
                change += newChange;
            }
            summedPositionChange += change / 2;
        }
    }

    return summedPositionChange / (numberOfNodes * numberOfNodes);
}
