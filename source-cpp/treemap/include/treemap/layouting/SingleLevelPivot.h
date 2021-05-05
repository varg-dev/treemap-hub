
#pragma once


#include <vector>

#include <treemap/treemap_api.h>


template <typename T>
class LinearizedBuffer;

class LinearizedTree;
class LinearizedTreeNode;
class Rect;


namespace pivotAlgorithms
{


using nodeIterator = const LinearizedTreeNode*;
// pivotFunc(rect, begin, end, weights, totalWeight) => pivotIterator
using pivotFunction = nodeIterator(const Rect &, const nodeIterator &, const nodeIterator &, const LinearizedBuffer<float> &, const float &);


TREEMAP_API nodeIterator pivotBySize(const Rect & rect, const nodeIterator & begin, const nodeIterator & end, const LinearizedBuffer<float> & weights, const float & totalWeight);

TREEMAP_API nodeIterator pivotByMiddle(const Rect & rect, const nodeIterator & begin, const nodeIterator & end, const LinearizedBuffer<float> & weights, const float & totalWeight);

TREEMAP_API nodeIterator pivotBySplitSize(const Rect & rect, const nodeIterator & begin, const nodeIterator & end, const LinearizedBuffer<float> & weights, const float & totalWeight);


} // namespace pivotAlgorithms


class TREEMAP_API SingleLevelPivot
{
public:
    static void layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, pivotAlgorithms::pivotFunction pivotFunc);
};
