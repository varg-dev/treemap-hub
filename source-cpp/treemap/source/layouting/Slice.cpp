
#include <treemap/layouting/Slice.h>

#include <treemap/layout/Rect.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


Slice::Slice(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight)
: AbstractRow(tree, weights, availableSpace, availableWeight, availableSpace.isHorizontal())
{
}

Slice::Slice(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal)
: AbstractRow(tree, weights, availableSpace, availableWeight, horizontal)
{
}

void Slice::layoutNodes(LinearizedBuffer<Rect> & layout) const
{
    auto offset = 0.0f;

    for (auto current = m_firstChild; current != m_lastChild; ++current)
    {
        const auto length = m_availableWeight > 0.0f ? m_weights[current] / m_availableWeight : 0.0f;

        layout[current] = m_availableSpace.relativeRect(offset, length, m_horizontal);

        offset += length;
    }
}
