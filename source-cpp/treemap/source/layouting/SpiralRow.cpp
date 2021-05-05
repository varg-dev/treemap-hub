
#include <treemap/layouting/SpiralRow.h>

#include <algorithm>
#include <numeric>

#include <glm/common.hpp>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


SpiralRow::SpiralRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight)
: Row(tree, weights, availableSpace, availableWeight)
, m_backside(false)
{
}

void SpiralRow::layoutNodes(LinearizedBuffer<Rect> & layout) const
{
    auto pos = 0.0f;
    const auto width = m_availableWeight > 0.0f ? std::min(m_childrenWeight / m_availableWeight, 1.0f) : 0.0f;

    for (auto current = m_firstChild; current != m_lastChild; ++current)
    {
        const auto weight = m_weights[current];
        const auto length = m_childrenWeight > 0.0f ? (weight / m_childrenWeight) : 0.0f;
        const auto startLength = std::max(pos, 0.0f);
        const auto startWidth = std::max(m_backside ? (1.0f - width) : 0.0f, 0.0f);

        const auto rect = m_horizontal
            ? m_availableSpace.relativeRect(glm::vec2(startLength, startWidth), glm::vec2(std::min(length, 1.0f), width))
            : m_availableSpace.relativeRect(glm::vec2(startWidth, startLength), glm::vec2(width, std::min(length, 1.0f)));

        layout[current] = std::move(rect);

        pos += length;
    }
}

void SpiralRow::next()
{
    Row::next(!m_horizontal);

    if (!m_horizontal)
        m_backside = !m_backside;
}

Rect SpiralRow::remainingSpace() const
{
    if (isDisabled())
    {
        return m_availableSpace.top(0.0f);
    }

    if (m_horizontal)
    {
        if (m_backside)
            return m_availableSpace.top(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
        else
            return m_availableSpace.bottom(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
    }
    else
    {
        if (m_backside)
            return m_availableSpace.left(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
        else
            return m_availableSpace.right(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
    }
}
