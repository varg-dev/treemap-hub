
#pragma once


#include <vector>

#include <treemap/layout/Rect.h>
#include <treemap/linearizedtree/LinearizedTree.h>

#include <treemap/treemap_api.h>


class TREEMAP_API SingleLevelStrip
{
public:
    static void layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, bool optimized = false);
};
