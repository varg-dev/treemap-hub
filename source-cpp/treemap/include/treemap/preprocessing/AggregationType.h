
#pragma once


#include <functional>
#include <map>


enum class AggregationType : unsigned int
{
    Sum,
    Average,
    Minimum,
    Maximum,
    Median,
    WeightedSum,
    WeightedAverage,
    WeightedOutlierAverage,
    WeightedOutlierWeightedAverage
};
