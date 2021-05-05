
#pragma once


#include <treemap/layout/Rect.h>

#include <treemap/treemap_api.h>


class LinearizedTree;
class LinearizedTreeNode;
template <typename T>
class LinearizedBuffer;


class TREEMAP_API AbstractRow
{
public:
    AbstractRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal);

    virtual void insert(const LinearizedTreeNode * node, float weight);

    virtual void layoutNodes(LinearizedBuffer<Rect> & layout) const = 0;
public:
    bool isDisabled() const;
    float availableWeight() const;
    const Rect & availableSpace() const;
    bool isHorizontal() const;

protected:
    const LinearizedTree * m_tree;
    const LinearizedBuffer<float> & m_weights;
    Rect m_availableSpace;
    float m_availableWeight;
    bool m_horizontal;
    const LinearizedTreeNode * m_firstChild; // inclusive
    const LinearizedTreeNode * m_lastChild; // exclusive
};
