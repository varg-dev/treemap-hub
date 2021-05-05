
#pragma once


#include <functional>

#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/treemap_api.h>


class LinearizedTree;


class TREEMAP_API AttributeSumAggregation
{
public:
    AttributeSumAggregation();

    void initialize();
    void deinitialize();

    void process(const LinearizedTree * tree, LinearizedBuffer<float> & values);
};
