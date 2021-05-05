
#include <treemap/preprocessing/AttributeSumAggregation.h>

#include <cmath>
#include <numeric>
#include <algorithm>

#include <glm/common.hpp>
#include <glm/exponential.hpp>

#include <treemap/preprocessing/AggregationType.h>
#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


namespace
{


inline float sum_accum(const LinearizedBuffer<float> & buffer, const LinearizedTree * tree, const LinearizedTreeNode * parent, const LinearizedTreeNode * first, const LinearizedTreeNode * end)
{
    auto sum = 0.0f;

    for (auto current = first; current != end; ++current)
    {
        sum += buffer[current];
    }

    return sum;
}


} // namespace


AttributeSumAggregation::AttributeSumAggregation()
{
}

void AttributeSumAggregation::initialize()
{
}

void AttributeSumAggregation::deinitialize()
{
}

void AttributeSumAggregation::process(const LinearizedTree * tree, LinearizedBuffer<float> & values)
{
    tree->reverseParentsDo([&](const LinearizedTreeNode * node) {
        if (node->isLeaf())
        {
            return;
        }

        values[node] = sum_accum(values, tree, node, tree->at(node->firstChild()), tree->at(node->firstChild()) + node->numberOfChildren());
    });
}
