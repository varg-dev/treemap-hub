
#pragma once


#include <glm/vec4.hpp>

#include <treemap/layouting/WeightPartitioning.h>
#include <treemap/layouting/RectangularLayoutAlgorithm.h>

#include <treemap/treemap_api.h>


class LinearizedTree;
class LinearizedTreeNode;
class Rect;
template <typename T>
class LinearizedBuffer;


class TREEMAP_API TreemapLayouter
{
public:
    TreemapLayouter();
    ~TreemapLayouter();

    void initialize();
    void deinitialize();

    void setAlgorithm(RectangularLayoutAlgorithm algorithm);

    void process(const LinearizedTree * tree, const LinearizedBuffer<float> & weights,
            LinearizedBuffer<Rect> & layout);

protected:
    RectangularLayoutAlgorithm m_algorithm;

protected:
    void singleNodeProcess(const LinearizedTree * tree, const LinearizedTreeNode * parent, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout);
};
