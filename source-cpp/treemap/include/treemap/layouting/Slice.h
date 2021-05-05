
#pragma once


#include <treemap/layouting/AbstractRow.h>

#include <treemap/treemap_api.h>


class TREEMAP_API Slice : public AbstractRow
{
public:
    Slice(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight);

    Slice(const LinearizedTree * tree, const LinearizedBuffer<float> & weights, const Rect & availableSpace, float availableWeight, bool horizontal);

    virtual void layoutNodes(LinearizedBuffer<Rect> & layout) const override;
};
