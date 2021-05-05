
#pragma once


#include <vector>


#include <treemap/layout/Rect.h>
#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/treemap_api.h>


class Node;

class TREEMAP_API SingleLevelApproximation
{
public:
    static void layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout);
};
