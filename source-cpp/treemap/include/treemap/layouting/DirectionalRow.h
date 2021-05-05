
#pragma once


#include <functional>

#include <treemap/layouting/Row.h>
#include <treemap/layout/Rect.h>
#include <treemap/layout/RectangularTreemapLayout.h>

#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/treemap_api.h>


class TREEMAP_API DirectionalRow : public Row
{
public:
    DirectionalRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight);
    DirectionalRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal);
    DirectionalRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal, bool reverse, bool parentReverse);

    void next(bool horizontal);

    bool reverse() const;
    void setReverse(bool reverse);

    bool parentReverse() const;
    void setParentReverse(bool reverse);

    virtual Rect remainingSpace() const override;

    virtual void layoutNodes(RectangularTreemapLayout & layout) const override;
protected:
    bool m_reverse;
    bool m_parentReverse;
};
