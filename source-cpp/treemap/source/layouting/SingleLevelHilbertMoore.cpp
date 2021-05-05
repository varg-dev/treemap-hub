#include <treemap/layouting/SingleLevelHilbertMoore.h>

#include <treemap/linearizedtree/LinearizedTree.h>

#include <treemap/layout/Rect.h>

#include <treemap/layouting/HilbertMoorePattern.h>
#include <treemap/layouting/WeightPartitioning.h>

#include <limits>
#include <cmath>
#include <algorithm>
#include <numeric>

#include <iostream>

#ifndef NDEBUG
#define LAYOUTER_INLINE
#else
#define LAYOUTER_INLINE inline
#endif


namespace
{


enum class CurveType
{
    None,
    Hilbert,
    Moore
};

// define Dissector 1-4
using Dissector1 = DissectorImpl<1,DoNothing>;

template<CurveType O> struct Dissector2 : DissectorImpl<2,SnakeH2> {};
template<> struct Dissector2<CurveType::Hilbert> : DissectorImpl<2,SnakeH2> {};
template<> struct Dissector2<CurveType::Moore> : DissectorImpl<2,SnakeM2> {};

template<CurveType O> struct Dissector3 : DissectorImpl<3,Snake3,Most13,Most23,Most33> {};
template<> struct Dissector3<CurveType::Hilbert> : DissectorImpl<3,Snake3,Most23> {};
template<> struct Dissector3<CurveType::Moore> : DissectorImpl<3,Most13,Most33> {};

template<CurveType O> struct Dissector4 : DissectorImpl<4,Snake4,Most14,Most24,Most34,Most44,Horizontal4,Vertical4> {};
template<> struct Dissector4<CurveType::Hilbert> : DissectorImpl<4,Snake4,Most24,Most34,Horizontal4> {};
template<> struct Dissector4<CurveType::Moore> : DissectorImpl<4,Most14,Most44,Vertical4> {};


template <CurveType orientation>
LAYOUTER_INLINE std::vector<Rect> layoutItems(const std::vector<float> & weights, const Rect & rect)
{
    assert(weights.size() >= 1 && weights.size() <= 4);

    switch (weights.size())
    {
    case 1:
        return Dissector1::dissectWithBestAR(weights, rect);
    case 2:
        return Dissector2<orientation>::dissectWithBestAR(weights, rect);
    case 3:
        return Dissector3<orientation>::dissectWithBestAR(weights, rect);
    case 4:
        return Dissector4<orientation>::dissectWithBestAR(weights, rect);
    default:
        assert(false);
        return {};
    }
}

void layoutRecursively(const Rect &rect, const LinearizedBuffer<float> &weights, LinearizedBuffer<Rect> &layout, const LinearizedTreeNode::NodeRange range, bool useMoore, PartitionAlg alg)
{
    auto size = static_cast<std::size_t>(range.last - range.first);

    assert(size > 0);

    if (size == 1)
    {
        layout[range.first] = rect;

        return;
    }

    if (size <= 4)
    {
        auto intermediateWeights = std::vector<float>(&weights.at(range.first), &weights.at(range.first)+size);

        if (rect.curveDirection() == Rect::CCW)
        {
            std::reverse(intermediateWeights.begin(), intermediateWeights.end());
        }

        auto rectangles = useMoore
            ? layoutItems<CurveType::Moore>(intermediateWeights, rect)
            : layoutItems<CurveType::Hilbert>(intermediateWeights, rect);

        assert(size == rectangles.size());

        if (rect.curveDirection() == Rect::CCW)
        {
            std::reverse(rectangles.begin(), rectangles.end());
        }

        std::copy(rectangles.begin(), rectangles.begin()+size, &layout.at(range.first));

        return;
    }

    // size > 4

    std::vector<LinearizedTreeNode::NodeRange> quadrants;
    if(alg == PartitionAlg::Greedy)
        quadrants = partitionGreedy(range, weights);
    else if(alg == PartitionAlg::MinMax)
        quadrants = partitionMinMax(range, weights);
    else
        quadrants = partitionVariance(range, weights);

    std::vector<float> quadrantWeights;
    for (const auto & chunk : quadrants)
    {
        auto it = weights.begin() + chunk.first->index();
        quadrantWeights.push_back(std::accumulate(it, it+chunk.size(), 0.0));
    }

    if (rect.curveDirection() == Rect::CCW)
    {
        std::reverse(quadrantWeights.begin(), quadrantWeights.end());
    }

    auto rectangles = useMoore
        ? layoutItems<CurveType::Moore>(quadrantWeights, rect)
        : layoutItems<CurveType::Hilbert>(quadrantWeights, rect);

    if (rect.curveDirection() == Rect::CCW)
    {
        std::reverse(rectangles.begin(), rectangles.end());
    }

    for (auto i = std::size_t(0); i < quadrants.size(); ++i)
    {
        layoutRecursively(rectangles[i], weights, layout, quadrants[i], false, alg);
    }
}


} // namespace


void SingleLevelHilbertMoore::layoutHilbert(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, PartitionAlg alg)
{
    layoutRecursively(rect, weights, layout, tree->childrenAsRange(parent), false, alg);
}

void SingleLevelHilbertMoore::layoutMoore(const LinearizedTreeNode *parent, const Rect &rect, const LinearizedTree * tree, const LinearizedBuffer<float> &weights, LinearizedBuffer<Rect> &layout, PartitionAlg alg)
{
    layoutRecursively(rect, weights, layout, tree->childrenAsRange(parent), true, alg);
}
