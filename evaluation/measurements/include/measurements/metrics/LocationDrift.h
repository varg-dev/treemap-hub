
#pragma once


#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/layout/RectangularTreemapLayout.h>

#include <measurements/measurements_api.h>


class MEASUREMENTS_API LocationDrift
{
public:
    LocationDrift();

    float compute(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
                  const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds);

protected:
    float locationDrift(const std::vector<LinearizedTree *> & trees, std::uint32_t maxId, const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids,
                        const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds, const std::vector<LinearizedBuffer<Rect>> & layouts);
};
