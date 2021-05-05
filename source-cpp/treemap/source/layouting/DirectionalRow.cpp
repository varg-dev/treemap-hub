
#include <treemap/layouting/DirectionalRow.h>

#include <algorithm>
#include <numeric>

#include <glm/common.hpp>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


DirectionalRow::DirectionalRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight)
: Row(tree, weights, availableSpace, availableWeight)
, m_reverse(false)
, m_parentReverse(false)
{
}

DirectionalRow::DirectionalRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal)
: Row(tree, weights, availableSpace, availableWeight, horizontal)
, m_reverse(false)
, m_parentReverse(false)
{
}

DirectionalRow::DirectionalRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal, bool reverse, bool parentReverse)
: Row(tree, weights, availableSpace, availableWeight, horizontal)
, m_reverse(reverse)
, m_parentReverse(parentReverse)
{
}

void DirectionalRow::layoutNodes(RectangularTreemapLayout & layout) const
{
    if (m_availableSpace.area() <= 0.0f || m_availableWeight <= 0.0f) {
        for (auto current = m_firstChild; current != m_lastChild; ++current)
        {
            layout[current] = Rect(m_availableSpace.pos, glm::vec2(0.0f, 0.0f));
        }

        return;
    }

    auto pos = 0.0f;
    const auto otherExtent = m_availableWeight > 0.0f ? std::min(m_childrenWeight / m_availableWeight, 1.0f) : 0.0f;

    for (auto current = m_firstChild; current != m_lastChild; ++current)
    {
        const auto weight = m_weights[current];
        const auto length = m_childrenWeight > 0.0f ? (weight / m_childrenWeight) : 0.0f;
        const auto start = std::max(m_reverse ? (1.0f - pos - length) : pos, 0.0f);
        const auto otherStart = std::max(m_parentReverse ? (1.0f - otherExtent) : 0.0f, 0.0f);
        // const auto rectOrientation = m_reverse ? Rect::DC : Rect::CD; // TODO: handle orientation

        const auto rect = m_horizontal
            ? m_availableSpace.relativeRect(glm::vec2(start, otherStart), glm::vec2(std::min(length, 1.0f), otherExtent))
            : m_availableSpace.relativeRect(glm::vec2(otherStart, start), glm::vec2(otherExtent, std::min(length, 1.0f)));

        layout[current] = std::move(rect);

        pos += length;

        assert(weight > 0 || layout[current].area() == 0);
    }
}

Rect DirectionalRow::remainingSpace() const
{
    if (isDisabled())
    {
        return m_availableSpace.top(0.0f);
    }

    if (m_horizontal)
    {
        if (m_parentReverse) {
            return m_availableSpace.top(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
        } else {
            return m_availableSpace.bottom(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
        }
    }
    else
    {
        if (m_parentReverse) {
            return m_availableSpace.left(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
        } else {
            return m_availableSpace.right(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
        }
    }
}

void DirectionalRow::next(bool horizontal)
{
    Row::next(horizontal);

    m_reverse = !m_reverse;
}

void DirectionalRow::setReverse(bool reverse)
{
    m_reverse = reverse;
}

bool DirectionalRow::reverse() const
{
    return m_reverse;
}

void DirectionalRow::setParentReverse(bool reverse)
{
    m_parentReverse = reverse;
}

bool DirectionalRow::parentReverse() const
{
    return m_parentReverse;
}
