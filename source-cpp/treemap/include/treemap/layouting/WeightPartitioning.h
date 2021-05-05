
#pragma once

#include <vector>
#include <utility>

#include <treemap/linearizedtree/LinearizedTreeNode.h>
#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/treemap_api.h>

enum class PartitionAlg {
    Greedy,
    MinMax,
    Variance
};

TREEMAP_API std::vector<LinearizedTreeNode::NodeRange> partitionMinMax(const LinearizedTreeNode::NodeRange & range, const LinearizedBuffer<float> &weights);

TREEMAP_API std::vector<LinearizedTreeNode::NodeRange> partitionVariance(const LinearizedTreeNode::NodeRange & range, const LinearizedBuffer<float> &weights);

TREEMAP_API std::vector<LinearizedTreeNode::NodeRange> partitionGreedy(const LinearizedTreeNode::NodeRange & range, const LinearizedBuffer<float> &weights);
