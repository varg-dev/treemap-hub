
#include <treemap/layouting/SingleLevelPivot.h>

#include <numeric>
#include <algorithm>

#include <glm/common.hpp>
#include <glm/exponential.hpp>

#include <treemap/layout/Rect.h>
#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


namespace
{

using pivotAlgorithms::nodeIterator;
using pivotAlgorithms::pivotFunction;

std::pair<nodeIterator, float> findL2End(const Rect & rect, const nodeIterator & /*begin*/, const nodeIterator & end, const nodeIterator & pivot, const LinearizedBuffer<float> & weights, const float & totalWeight)
{
    const auto l2Begin = pivot + 1;
    auto l2End = l2Begin;

    const auto pWeight = weights[pivot];
    auto pl2Weight = pWeight;

    // |     d    |
    //     | c|
    // +---+--+---+ -
    // |   |P |   | a
    // | L1+--+L3 | -
    // |   |L2|   | b
    // +---+--+---+ -
    //
    // pivotRatio  = c/a
    //             = c/d / a/d
    //             = c(a+b)/d(a+b) / (a/(a+b) / d/(a+b))
    //             = c(a+b)/d(a+b) / (ca/c(a+b) / d/(a+b))
    //             = c(a+b)/d(a+b) * (d/(a+b) / ca/c(a+b))
    //             = c(a+b)/d(a+b) * d/(a+b) * c(a+b)/ca
    //             = c(a+b)/d(a+b) * c(a+b)/ca * d/(a+b)
    //             = pl2Weight/totalWeight * pl2Weight/pWeight * rect.aspectRatio()
    // pl2Weight^2 = pivotRatio * totalWeight * pWeight / rect.aspectRatio()

    const auto pl2IdealWeight = glm::sqrt((pWeight * totalWeight) / rect.aspectRatio());

    while (l2End != end && (pl2Weight + weights[l2End]) < pl2IdealWeight)
    {
        pl2Weight += weights[l2End++];
    }

    if (l2End != end)
    {
        const auto pl2ToTotalLo =  pl2Weight                    / totalWeight;
        const auto pl2ToTotalHi = (pl2Weight + weights[l2End]) / totalWeight;
        const auto pl2ToPivotLo =  pl2Weight                    / pWeight;
        const auto pl2ToPivotHi = (pl2Weight + weights[l2End]) / pWeight;
        auto aspectRatioPLo = pl2ToTotalLo * pl2ToPivotLo * rect.aspectRatio();
        auto aspectRatioPHi = pl2ToTotalHi * pl2ToPivotHi * rect.aspectRatio();

        if (aspectRatioPLo < float(1.0))
            aspectRatioPLo = float(1.0) / aspectRatioPLo;
        if (aspectRatioPHi < float(1.0))
            aspectRatioPHi = float(1.0) / aspectRatioPHi;

        if (aspectRatioPLo > aspectRatioPHi)
        {
            pl2Weight += weights[l2End++];
        }
    }

    // TODO: Switch to structured bindings, once using C++17
    return std::make_pair(l2End, pl2Weight - pWeight);
}

std::array<Rect, 4> calculateSpaces(const Rect & totalSpace, const float l1Weight, const float  pWeight, const float l2Weight, const float l3Weight)
{
    const auto pl2Weight = pWeight + l2Weight;
    const auto totalWeight = l1Weight + pWeight + l2Weight + l3Weight;
    if (totalSpace.isHorizontal())
    {
        const auto l1Height = totalSpace.height();
        const auto pHeight  = totalSpace.height() * (pl2Weight > std::numeric_limits<float>::epsilon() ?  pWeight / pl2Weight : float(1.0));
        const auto l2Height = totalSpace.height() * (pl2Weight > std::numeric_limits<float>::epsilon() ? l2Weight / pl2Weight : float(0.0));
        const auto l3Height = totalSpace.height();

        const auto l1Width = totalSpace.width() * (totalWeight > std::numeric_limits<float>::epsilon() ?  l1Weight / totalWeight : float(0.0));
        const auto pWidth  = totalSpace.width() * (totalWeight > std::numeric_limits<float>::epsilon() ? pl2Weight / totalWeight : float(1.0));
        const auto l2Width = totalSpace.width() * (totalWeight > std::numeric_limits<float>::epsilon() ? pl2Weight / totalWeight : float(1.0));
        const auto l3Width = totalSpace.width() * (totalWeight > std::numeric_limits<float>::epsilon() ?  l3Weight / totalWeight : float(0.0));

        const auto l1Pos = static_cast<glm::vec2>(totalSpace.pos);
        const auto pPos  = static_cast<glm::vec2>(totalSpace.pos) + glm::vec2(l1Width, 0.0);
        const auto l2Pos = static_cast<glm::vec2>(totalSpace.pos) + glm::vec2(l1Width, pHeight);
        const auto l3Pos = static_cast<glm::vec2>(totalSpace.pos) + glm::vec2(l1Width + l2Width, 0.0);

        return {{
            Rect{l1Pos.x, l1Pos.y, l1Width, l1Height},
            Rect{ pPos.x,  pPos.y,  pWidth,  pHeight},
            Rect{l2Pos.x, l2Pos.y, l2Width, l2Height},
            Rect{l3Pos.x, l3Pos.y, l3Width, l3Height}
        }};
    }
    else
    {
        const auto l1Height = totalSpace.height() * (totalWeight > std::numeric_limits<float>::epsilon() ?  l1Weight / totalWeight : float(0.0));
        const auto pHeight  = totalSpace.height() * (totalWeight > std::numeric_limits<float>::epsilon() ? pl2Weight / totalWeight : float(1.0));
        const auto l2Height = totalSpace.height() * (totalWeight > std::numeric_limits<float>::epsilon() ? pl2Weight / totalWeight : float(1.0));
        const auto l3Height = totalSpace.height() * (totalWeight > std::numeric_limits<float>::epsilon() ?  l3Weight / totalWeight : float(0.0));

        const auto l1Width = totalSpace.width();
        const auto pWidth  = totalSpace.width() * (pl2Weight > std::numeric_limits<float>::epsilon() ?  pWeight / pl2Weight : float(1.0));
        const auto l2Width = totalSpace.width() * (pl2Weight > std::numeric_limits<float>::epsilon() ? l2Weight / pl2Weight : float(0.0));
        const auto l3Width = totalSpace.width();

        const auto l1Pos = static_cast<glm::vec2>(totalSpace.pos);
        const auto pPos  = static_cast<glm::vec2>(totalSpace.pos) + glm::vec2(0.0, l1Height);
        const auto l2Pos = static_cast<glm::vec2>(totalSpace.pos) + glm::vec2(pWidth, l1Height);
        const auto l3Pos = static_cast<glm::vec2>(totalSpace.pos) + glm::vec2(0.0, l1Height + l2Height);

        return {{
            Rect{l1Pos.x, l1Pos.y, l1Width, l1Height},
            Rect{ pPos.x,  pPos.y,  pWidth,  pHeight},
            Rect{l2Pos.x, l2Pos.y, l2Width, l2Height},
            Rect{l3Pos.x, l3Pos.y, l3Width, l3Height}
        }};
    }
}

void pivotStep(const Rect & rect, const LinearizedTreeNode* begin, const LinearizedTreeNode* end, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, pivotFunction pivotFunc)
{
    const auto totalWeight = std::accumulate(begin, end, float(0.0), [&weights](const float & currentSum, const LinearizedTreeNode & nextNode) {
        return currentSum + weights[nextNode.index()];
    });

    // Determine pivot element
    const auto pivotIt = pivotFunc(rect, begin, end, weights, totalWeight);

    // Determine L2
    const auto l2Begin = pivotIt + 1;
    const auto l2EndWeight = findL2End(rect, begin, end, pivotIt, weights, totalWeight);
    const auto l2End = std::get<0>(l2EndWeight);

    // Calculate spaces for L1, P, L2, L3
    const auto l1Weight = std::accumulate(begin, pivotIt, float(0.0), [&weights](const float & currentSum, const LinearizedTreeNode & nextNode){
        return currentSum + weights[nextNode.index()];
    });
    const auto  pWeight = weights[pivotIt];
    const auto l2Weight = std::get<1>(l2EndWeight);
    const auto l3Weight = totalWeight - (l1Weight + pWeight + l2Weight);

    const auto spaces = calculateSpaces(rect, l1Weight, pWeight, l2Weight, l3Weight);

    layout[pivotIt] = std::get<1>(spaces); // P
    if (pivotIt > begin) // L1
    {
        pivotStep(std::get<0>(spaces), begin, pivotIt, weights, layout, pivotFunc);
    }
    if (l2End > l2Begin) // L2
    {
        pivotStep(std::get<2>(spaces), l2Begin, l2End, weights, layout, pivotFunc);
    }
    if (end > l2End) // L3
    {
        pivotStep(std::get<3>(spaces), l2End, end, weights, layout, pivotFunc);
    }
}

} // namespace


namespace pivotAlgorithms
{


namespace
{


float ratioL1L3ForPivot(const Rect & rect, const nodeIterator & begin, const nodeIterator & end, const nodeIterator & pivot, const LinearizedBuffer<float> & weights, const float & totalWeight)
{
    //const auto l2Begin = pivot + 1;
    const auto l2EndWeight = findL2End(rect, begin, end, pivot, weights, totalWeight);
    //const auto l2End = std::get<0>(l2EndWeight);

    const auto l1Weight = std::accumulate(begin, pivot, float(0.0), [&weights](const float & currentSum, const LinearizedTreeNode & nextNode) {
        return currentSum + weights[nextNode.index()];
    });
    const auto l2Weight = std::get<1>(l2EndWeight);
    const auto l3Weight = totalWeight - (l1Weight + weights[pivot] + l2Weight);

    return glm::max(l1Weight / l3Weight, l3Weight / l1Weight);
}

} // namespace


nodeIterator pivotBySize(const Rect & /*rect*/, const nodeIterator & begin, const nodeIterator & end, const LinearizedBuffer<float> & weights, const float & /*totalWeight*/)
{
    return std::max_element(begin, end, [&weights](const LinearizedTreeNode & n1, const LinearizedTreeNode & n2) {
        return weights[n1.index()] < weights[n2.index()];
    });
}

nodeIterator pivotByMiddle(const Rect & /*rect*/, const nodeIterator & begin, const nodeIterator & end, const LinearizedBuffer<float> & /*weights*/, const float & /*totalWeight*/)
{
    return begin + (end - begin) / 2;
}

nodeIterator pivotBySplitSize(const Rect & rect, const nodeIterator & begin, const nodeIterator & end, const LinearizedBuffer<float> & weights, const float & totalWeight)
{
    auto bestRatio = std::numeric_limits<float>::max();
    auto bestPivot = begin;

    for (auto pivot = begin; pivot < end; pivot++)
    {
        const auto ratio = ratioL1L3ForPivot(rect, begin, end, pivot, weights, totalWeight);

        if (ratio < bestRatio)
        {
            bestRatio = ratio;
            bestPivot = pivot;
        }
    }

    return bestPivot;
}


} // namespace pivotAlgorithms


void SingleLevelPivot::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, pivotAlgorithms::pivotFunction pivotFunc)
{
    pivotStep(rect, tree->at(parent->firstChild()), tree->at(parent->firstChild()) + parent->numberOfChildren(), weights, layout, pivotFunc);
}
