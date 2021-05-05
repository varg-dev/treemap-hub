
#pragma once


#include <functional>

#include <treemap/layouting/Row.h>

#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/treemap_api.h>


class TREEMAP_API SpiralRow : public Row
{
public:
    SpiralRow(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight);

    void next();

    virtual void layoutNodes(LinearizedBuffer<Rect> & layout) const override;

    virtual Rect remainingSpace() const override;

protected:
    bool m_backside;
};
