from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio

from enum import Enum

class Metric(str, Enum):
    aar_each = "average-aspect-ratio-each"
    aar_avg = "average-aspect-ratio-average"
    aar_median = "average-aspect-ratio-median"
    aar_min = "average-aspect-ratio-minimum"
    aar_max = "average-aspect-ratio-maximum"
    rdc = "relative-direction-change"
    rdc_ri = "relative-direction-change-rotation-invariant"
    ld = "location-drift"

class FunctionProxy:
    def __init__(self, function):
        self.function = function

    def __call__(self, *args, **kwargs):
        return self.function(*args, **kwargs)

class Metric_Function(Enum):
    aar_each = FunctionProxy(averageAspectRatio.averageAspectRatioEach)
    aar_avg = FunctionProxy(averageAspectRatio.averageAspectRatioAverage)
    aar_median = FunctionProxy(averageAspectRatio.averageAspectRatioMedian)
    aar_min = FunctionProxy(averageAspectRatio.averageAspectRatioMin)
    aar_max = FunctionProxy(averageAspectRatio.averageAspectRatioMax)
    rdc = FunctionProxy(relativeDirectionChange.rdc)
    rdc_ri = FunctionProxy(relativeDirectionChange.rdc_ri)
    ld = FunctionProxy(locationDrift.meanLocationDrift)

def data_metrics(metric: Metric, layouts):
    for m in Metric:
        print(m)
    return {
        "Dataset": [x for x in layouts],
        metric.value: Metric_Function[metric.name].value(list(layouts.values()))}


def data_comparison(metric: Metric, layouts1, layouts2):

    try:
        layoutMetric1 = Metric_Function[metric.name].value(list(layouts1.values()))
        layoutMetric2 = Metric_Function[metric.name].value(list(layouts2.values()))
        betterLayout = "first" if (layoutMetric1 < layoutMetric2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser "+ metric.value,
                "first dataset": [x for x in layouts1],
                metric.value+" first dataset": layoutMetric1,
                "second dataset": [x for x in layouts2],
                metric.value+" second dataset": layoutMetric2
                }
    except:
        return {metric.value + "can not be used for comparison"}
  
            
def data_fullreport(layouts):
    result = {
        "Dataset": [x for x in layouts]  
    }
    for metric in Metric:
        result[metric.value] = Metric_Function[metric.name].value(list(layouts.values()))
    
    return result