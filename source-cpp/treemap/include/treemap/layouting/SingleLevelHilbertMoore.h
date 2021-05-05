
#pragma once


#include <vector>
#include <array>

#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/layout/Rect.h>
#include <treemap/layouting/WeightPartitioning.h>

#include <treemap/treemap_api.h>


class LinearizedTree;


class TREEMAP_API SingleLevelHilbertMoore
{
public:
    static void layoutHilbert(const LinearizedTreeNode *parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, PartitionAlg alg);
    static void layoutMoore(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, PartitionAlg alg);
};
