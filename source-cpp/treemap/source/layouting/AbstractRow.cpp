
#include <treemap/layouting/AbstractRow.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


AbstractRow::AbstractRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal)
: m_tree(tree)
, m_weights(weights)
, m_availableSpace(availableSpace)
, m_availableWeight(availableWeight)
, m_horizontal(horizontal)
, m_firstChild(nullptr)
, m_lastChild(nullptr) // exclusive
{
}

bool AbstractRow::isDisabled() const
{
    return m_availableWeight <= 0.0f || m_availableSpace.extent.x <= 0.0f || m_availableSpace.extent.y <= 0.0f;
}

void AbstractRow::insert(const LinearizedTreeNode * node, float /*weight*/)
{
    if (m_firstChild == nullptr)
    {
        m_firstChild = node;
    }

    m_lastChild = node+1;
}

float AbstractRow::availableWeight() const
{
    return m_availableWeight;
}

const Rect & AbstractRow::availableSpace() const
{
    return m_availableSpace;
}

bool AbstractRow::isHorizontal() const
{
    return m_horizontal;
}
