
#include <treemap-measurements/metrics/LocationDrift.h>


#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include <cppassist/threading/parallelfor.h>

#include <treemap-layouting/layout/Rect.h>


LocationDrift::LocationDrift()
{
}


float LocationDrift::compute(const std::vector<LinearizedTree *> & trees, const std::vector<LinearizedBuffer<Rect>> & layouts, std::uint32_t maxId,
              const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds)
{
    return locationDrift(trees, maxId, index2ids, nodeIndexByIds, layouts);
}


float LocationDrift::locationDrift(const std::vector<LinearizedTree *> & trees, std::uint32_t maxId, const std::vector<LinearizedBuffer<std::uint32_t>> & index2ids, const std::vector<std::unordered_map<std::uint32_t, std::uint32_t>> & nodeIndexByIds, const std::vector<LinearizedBuffer<Rect>> & layouts)
{
    std::vector<glm::vec2> mergedCenters(maxId+1);
    std::vector<unsigned int> occurrencesPerNode(maxId+1);

    for (std::uint32_t i = 0; i < trees.size(); ++i)
    {
        const auto tree = trees[i];
        const auto & ids = index2ids[i];

        cppassist::forEach(std::uint32_t(0), tree->size(), [& mergedCenters, & occurrencesPerNode, tree, i, &layouts, &ids](std::uint32_t index) {
            const auto node = tree->at(index);

            assert(node != nullptr);

            if (layouts.at(i).at(index).area() >= glm::epsilon<float>())
            {
                mergedCenters.at(ids.at(node)) += layouts.at(i).at(index).center();
                occurrencesPerNode.at(ids.at(node)) += 1;
            }
        }, true);
    }

    cppassist::forEach(std::size_t(0), mergedCenters.size(), [& mergedCenters, & occurrencesPerNode](std::size_t index) {
        auto & center = mergedCenters.at(index);
        const auto occurrence = occurrencesPerNode.at(index);

        assert(!std::isnan(center.x));
        assert(!std::isnan(center.y));

        if (occurrence > 0)
        {
            center /= float(occurrence);
        }

        assert(!std::isnan(center.x));
        assert(!std::isnan(center.y));
    }, true);

    std::vector<float> locationDrifts(maxId+1);

    for (std::uint32_t i = 0; i < trees.size(); ++i)
    {
        const auto tree = trees.at(i);
        const auto & ids = index2ids[i];

        cppassist::forEach(std::uint32_t(0), tree->size(), [& mergedCenters, & locationDrifts, tree, i, & layouts, & ids](std::uint32_t index) {
            const auto node = tree->at(index);

            const auto & center = mergedCenters.at(ids.at(node));

            if (layouts.at(i).at(index).area() >= glm::epsilon<float>())
            {
                locationDrifts.at(ids.at(node)) += glm::distance(center, layouts.at(i).at(index).center());
            }
        }, true);
    }

    cppassist::forEach(std::size_t(0), locationDrifts.size(), [& locationDrifts, & occurrencesPerNode](std::size_t index) {
        auto & locationDrift = locationDrifts.at(index);
        const auto occurrence = occurrencesPerNode.at(index);

        if (occurrence > 0)
        {
            locationDrift /= float(occurrence);
        }
    }, true);

    auto summedLocationDrift = 0.0f;
    auto numberOfNodes = 0;

    for (auto i = size_t(0); i < locationDrifts.size(); ++i)
    {
        if (occurrencesPerNode.at(i) > 0)
        {
            summedLocationDrift += locationDrifts.at(i);

            assert(!std::isnan(summedLocationDrift));

            numberOfNodes += 1;
        }
    }

    return numberOfNodes >= 1 ? summedLocationDrift / numberOfNodes : 0.0f;
}
