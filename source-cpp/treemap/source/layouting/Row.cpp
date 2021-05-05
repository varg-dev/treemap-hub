
#include <treemap/layouting/Row.h>

#include <algorithm>
#include <numeric>

#include <glm/common.hpp>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


Row::Row(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight)
: AbstractRow(tree, weights, availableSpace, availableWeight, availableSpace.extent.x < availableSpace.extent.y)
, m_insertedChildrenCount(0)
, m_overallChildrenWeight(0)
, m_parentWeight(availableWeight)
, m_childrenWeight(0)
, m_maxChildrenWeight(0.0f)
, m_minChildrenWeight(std::numeric_limits<float>::infinity())
{
}

Row::Row(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal)
: AbstractRow(tree, weights, availableSpace, availableWeight, horizontal)
, m_insertedChildrenCount(0)
, m_overallChildrenWeight(0)
, m_parentWeight(availableWeight)
, m_childrenWeight(0)
, m_maxChildrenWeight(0.0f)
, m_minChildrenWeight(std::numeric_limits<float>::infinity())
{
}

void Row::insert(const LinearizedTreeNode * node, float weight)
{
    AbstractRow::insert(node, weight);

    if (weight >= std::numeric_limits<float>::epsilon())
    {
        // TODO: reintegrate assertions
        //assert(m_childrenWeight + weight <= m_availableWeight);
        //assert(m_overallChildrenWeight + weight <= m_parentWeight);

        m_childrenWeight = std::min(m_childrenWeight + weight, m_availableWeight);
        m_overallChildrenWeight = std::min(m_overallChildrenWeight + weight, m_parentWeight);
        m_minChildrenWeight = std::min(m_minChildrenWeight, weight);
        m_maxChildrenWeight = std::max(m_maxChildrenWeight, weight);
        ++m_insertedChildrenCount;
    }
}

void Row::layoutNodes(LinearizedBuffer<Rect> & layout) const
{
    if (m_availableSpace.area() <= 0.0f || m_availableWeight <= 0.0f) {
        for (auto current = m_firstChild; current != m_lastChild; ++current)
        {
            layout[current] = Rect(m_availableSpace.pos, glm::vec2(0.0f, 0.0f));
        }

        return;
    }

    auto pos = 0.0f;
    const auto otherExtent = std::min(m_childrenWeight / m_availableWeight, 1.0f);

    for (auto current = m_firstChild; current != m_lastChild; ++current)
    {
        const auto weight = m_weights[current];
        const auto length = m_childrenWeight > 0.0f ? weight / m_childrenWeight : 0.0f;

        if (m_horizontal)
        {
            layout[current] = m_availableSpace.relativeRect(glm::vec2(pos, 0.0f), glm::vec2(std::min(length, 1.0f), otherExtent));
        }
        else
        {
            layout[current] = m_availableSpace.relativeRect(glm::vec2(0.0f, pos), glm::vec2(otherExtent, std::min(length, 1.0f)));
        }

        pos += length;

        assert(weight > 0 || layout[current].area() == 0);
    }
}

bool Row::increasesWorstAspectRatio(float additionalWeight) const
{
    if (isDisabled() || m_firstChild == nullptr || additionalWeight < std::numeric_limits<float>::epsilon())
    {
        return false;
    }

    return worstAspectRatio(additionalWeight) > worstAspectRatio();
}

bool Row::increasesAverageAspectRatio(float additionalWeight, bool optimized) const
{
    if (isDisabled() || m_firstChild == nullptr || additionalWeight < std::numeric_limits<float>::epsilon())
    {
        return false;
    }

    if (optimized)
    {
        return optimizedAverageAspectRatio(additionalWeight) > optimizedAverageAspectRatio();
    }
    else
    {
        return averageAspectRatio(additionalWeight) > averageAspectRatio();
    }
}

float Row::worstAspectRatio() const
{
    const auto weightSum = m_childrenWeight;

    const auto primaryExtent = m_horizontal ? m_availableSpace.extent.x : m_availableSpace.extent.y;
    const auto secondaryExtent = (m_horizontal ? m_availableSpace.extent.y : m_availableSpace.extent.x) * weightSum / m_availableWeight;

    return std::max(
        aspectRatio(m_minChildrenWeight / weightSum, primaryExtent, secondaryExtent),
        aspectRatio(m_maxChildrenWeight / weightSum, primaryExtent, secondaryExtent)
    );
}

float Row::worstAspectRatio(float additionalWeight) const
{
    const auto weightSum = std::min(m_childrenWeight + additionalWeight, m_availableWeight);

    const auto primaryExtent = m_horizontal ? m_availableSpace.extent.x : m_availableSpace.extent.y;
    const auto secondaryExtent = (m_horizontal ? m_availableSpace.extent.y : m_availableSpace.extent.x) * weightSum / m_availableWeight;

    return std::max(
        aspectRatio(std::min(m_minChildrenWeight, additionalWeight) / weightSum, primaryExtent, secondaryExtent),
        aspectRatio(std::max(m_maxChildrenWeight, additionalWeight) / weightSum, primaryExtent, secondaryExtent)
    );
}

float Row::averageAspectRatio() const
{
    auto sumAspectRatio = 0.0f;
    auto count = 0u;

    computeAspectRatios(0.0f, [&sumAspectRatio, &count](float aspectRatio) {
        sumAspectRatio += aspectRatio;
        ++count;
    });

    return count == 0 ? 0.0f : sumAspectRatio / count;
}

float Row::averageAspectRatio(float additionalWeight) const
{
    auto sumAspectRatio = 0.0f;
    auto count = 0u;

    computeAspectRatios(additionalWeight, [&sumAspectRatio, &count](float aspectRatio) {
        sumAspectRatio += aspectRatio;
        ++count;
    });

    return count == 0 ? 0.0f : sumAspectRatio / count;
}

float Row::optimizedAverageAspectRatio() const
{
    const auto weightSum = m_childrenWeight;

    const auto primaryExtent = m_horizontal ? m_availableSpace.extent.x : m_availableSpace.extent.y;
    const auto secondaryExtent = (m_horizontal ? m_availableSpace.extent.y : m_availableSpace.extent.x) * weightSum / m_availableWeight;

    return aspectRatio(1.0f / m_insertedChildrenCount, primaryExtent, secondaryExtent);
}

float Row::optimizedAverageAspectRatio(float additionalWeight) const
{
    const auto weightSum = std::min(m_childrenWeight + additionalWeight, m_availableWeight);

    const auto primaryExtent = m_horizontal ? m_availableSpace.extent.x : m_availableSpace.extent.y;
    const auto secondaryExtent = (m_horizontal ? m_availableSpace.extent.y : m_availableSpace.extent.x) * weightSum / m_availableWeight;

    return aspectRatio(1.0f / (m_insertedChildrenCount+1), primaryExtent, secondaryExtent);
}

void Row::next()
{
    m_availableSpace = remainingSpace();
    m_availableWeight = remainingWeight();
    m_horizontal = m_availableSpace.isVertical();
    m_firstChild = nullptr;
    m_lastChild = nullptr;
    m_insertedChildrenCount = 0;
    //m_overallChildrenWeight stays the same
    //m_parentWeight stays the same
    m_childrenWeight = 0.0f;
    m_maxChildrenWeight = 0.0f;
    m_minChildrenWeight = std::numeric_limits<float>::infinity();
}

void Row::next(bool horizontal)
{
    m_availableSpace = remainingSpace();
    m_availableWeight = remainingWeight();
    m_horizontal = horizontal;
    m_firstChild = nullptr;
    m_lastChild = nullptr;
    m_insertedChildrenCount = 0;
    //m_overallChildrenWeight stays the same
    //m_parentWeight stays the same
    m_childrenWeight = 0.0f;
    m_maxChildrenWeight = 0.0f;
    m_minChildrenWeight = std::numeric_limits<float>::infinity();
}

Rect Row::remainingSpace() const
{
    if (isDisabled())
    {
        return m_availableSpace.top(0.0f);
    }

    if (m_horizontal)
    {
        return m_availableSpace.bottom(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
    }
    else
    {
        return m_availableSpace.right(1.0f - std::min(m_childrenWeight / m_availableWeight, 1.0f));
    }
}

float Row::remainingWeight() const
{
    return isDisabled() ? 0.0f : std::max(m_parentWeight - m_overallChildrenWeight, 0.0f);
}
