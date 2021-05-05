
#pragma once


#include <functional>

#include <treemap/buffers/LinearizedBuffer.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>

#include <treemap/layouting/AbstractRow.h>

#include <treemap/treemap_api.h>


class TREEMAP_API Row : public AbstractRow
{
public:
    Row(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight);
    Row(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal);

    virtual void insert(const LinearizedTreeNode * node, float weight);

    bool increasesWorstAspectRatio(float additionalWeight) const;
    bool increasesAverageAspectRatio(float additionalWeight, bool optimized = false) const;

    void next();
    void next(bool horizontal);

    virtual Rect remainingSpace() const;
    float remainingWeight() const;

    virtual void layoutNodes(LinearizedBuffer<Rect> & layout) const override;
protected:
    size_t m_insertedChildrenCount;
    float m_overallChildrenWeight;
    float m_parentWeight;
    float m_childrenWeight;
    float m_maxChildrenWeight;
    float m_minChildrenWeight;

    float worstAspectRatio() const;
    float worstAspectRatio(float additionalWeight) const;
    float averageAspectRatio() const;
    float averageAspectRatio(float additionalWeight) const;
    float optimizedAverageAspectRatio() const;
    float optimizedAverageAspectRatio(float additionalWeight) const;

    template <typename Callback>
    void computeAspectRatios(float additionalWeight, Callback callback) const;
};


namespace
{


inline float aspectRatio(float fraction, float primaryExtent, float secondaryExtent)
{
    const auto a = fraction * primaryExtent;
    const auto b = secondaryExtent;

    return a > b ? a/b : b/a;
}


}

template <typename Callback>
void Row::computeAspectRatios(float additionalWeight, Callback callback) const
{
    const auto weightSum = std::min(m_childrenWeight + additionalWeight, m_availableWeight);

    const auto primaryExtent = m_horizontal ? m_availableSpace.extent.x : m_availableSpace.extent.y;
    const auto secondaryExtent = (m_horizontal ? m_availableSpace.extent.y : m_availableSpace.extent.x) * weightSum / m_availableWeight;

    for (auto current = m_firstChild; current != m_lastChild; ++current)
    {
        const auto weight = m_weights[current];

        if (weight < std::numeric_limits<float>::epsilon())
            continue;

        callback(aspectRatio(weight / weightSum, primaryExtent, secondaryExtent));
    }

    if (additionalWeight >= std::numeric_limits<float>::epsilon())
    {
        callback(aspectRatio(additionalWeight / weightSum, primaryExtent, secondaryExtent));
    }
}
