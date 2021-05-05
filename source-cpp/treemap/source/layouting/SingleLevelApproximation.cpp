
#include <treemap/layouting/SingleLevelApproximation.h>

#include <numeric>
#include <algorithm>
#include <iostream>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree//LinearizedTreeNode.h>


namespace
{
void approximationStep(const Rect & rect
                     , const LinearizedTreeNode * begin
                     , const LinearizedTreeNode * end
                     , const LinearizedBuffer<float> & weights
                     , LinearizedBuffer<Rect> & layout)
{
    assert(begin != end);
    // usual assumption: children between begin & end are sorted by weight in ascending order

    auto totalWeight = std::accumulate(begin, end, float(0.0), [&weights](const float & currentSum, const LinearizedTreeNode & nextNode){return currentSum + weights[nextNode.index()];});

    // determine split point, so that both sides contain at least 1/3 of the total weight
    auto heaviestChild = end-1;
    const LinearizedTreeNode* splitPoint = nullptr;

    float firstHalfWeight;
    if (weights[heaviestChild] * 3.0 >= totalWeight)
    {
        // heaviest child has enought weight for proper split
        splitPoint = end-1;
        firstHalfWeight = totalWeight - weights[heaviestChild];
    }
    else
    {
        // collect children with small weights until reaching 1/3 limit
        splitPoint = begin;
        auto collectedWeight = float(0.0);

        while (collectedWeight * 3.0 < totalWeight)
        {
            collectedWeight += weights[splitPoint];
            splitPoint++;

            if (splitPoint == end) // Should never happen
            {
                std::cout << "[Treemap Demo] Approximation layout: Could not find proper split point!" << std::endl;
                return;
            }
        }

        firstHalfWeight = collectedWeight;
    }

    // split current rect according to found split, recurse
    auto splitRatio = totalWeight > std::numeric_limits<float>::epsilon() ? firstHalfWeight / totalWeight : float(0.5);

    auto firstHalfRect = rect.isHorizontal()
                       ? Rect(rect.pos, glm::vec2(rect.width() * splitRatio, rect.height()))
                       : Rect(rect.pos, glm::vec2(rect.width(), rect.height() * splitRatio));
    auto secondHalfRect = rect.isHorizontal()
                        ? Rect(rect.left() + rect.width() * splitRatio, rect.bottom(), rect.width() * (1 - splitRatio), rect.height())
                        : Rect(rect.left(), rect.bottom() + rect.height() * splitRatio, rect.width(), rect.height() * (1 - splitRatio));

    if (splitPoint - begin == 1)
    {
        layout[begin] = firstHalfRect;
    }
    else
    {
        approximationStep(firstHalfRect, begin, splitPoint, weights, layout);
    }

    if (end - splitPoint == 1)
    {
        layout[splitPoint] = secondHalfRect;
    }
    else
    {
        approximationStep(secondHalfRect, splitPoint, end, weights, layout);
    }
}

} // namespace


void SingleLevelApproximation::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout)
{
    // early out for single children
    if (parent->numberOfChildren() < 2)
    {
        layout[parent->firstChild()] = rect;

        return;
    }

    // expect sorted children
    //std::sort(tree->at(parent->firstChild()), tree->at(parent->firstChild()) + parent->numberOfChildren(), [&weights](const LinearizedTreeNode * a, const LinearizedTreeNode * b){return weights[a] < weights[b];});

    approximationStep(rect, tree->at(parent->firstChild()), tree->at(parent->firstChild()) + parent->numberOfChildren(), weights, layout);
}
