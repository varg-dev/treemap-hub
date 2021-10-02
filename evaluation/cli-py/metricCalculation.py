from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio

from enum import Enum

class Metric(str, Enum):
    aar = "average-aspect-ratio"
    rdc = "relative-direction-change"
    rdc_ri = "relative-direction-change-rotation-invariant"
    ld = "location-drift"

class FunctionProxy:
    def __init__(self, function):
        self.function = function

    def __call__(self, *args, **kwargs):
        return self.function(*args, **kwargs)

class Metric_Function(Enum):
    aar = FunctionProxy(averageAspectRatio.averageAspectRatio)
    rdc = FunctionProxy(relativeDirectionChange.rdc)
    rdc_ri = FunctionProxy(relativeDirectionChange.rdc_ri)
    ld = FunctionProxy(locationDrift.meanLocationDrift)

def data_metrics(metric: Metric, layouts):
    if metric == Metric.aar:
        result = []
        for i in layouts:
            aar = Metric_Function[metric.name].value(layouts[i])
            result.append({
                "Dataset": i,
                metric.value: aar
            })
        return {str(result)}
    
    return {
        "Dataset": [x for x in layouts],
        metric.value: Metric_Function[metric.name].value(list(layouts.values()))}


def data_comparison(metric: Metric, layouts1, layouts2):

    if metric == Metric.aar:
        aar1 = []
        aar2 = []

        avgAar1 = 0
        for i in layouts1:
            aar = averageAspectRatio.averageAspectRatio(layouts1[i])
            aar1.append({
                "Dataset": i,
                "average aspect ratio": aar
            })
            avgAar1 += aar
        avgAar1 /= len(layouts1)

        avgAar2 = 0
        for i in layouts2:
            aar = averageAspectRatio.averageAspectRatio(layouts2[i])
            aar2.append({
                "Dataset": i,
                "average aspect ratio": aar
            })
            avgAar2 += aar
        avgAar2 /= len(layouts1)

        betterLayout = "first" if (avgAar1 < avgAar1) else "second"        
        return {"result": "In average, the layouts from the " + betterLayout + " set of layouts had the better Average Aspect Ratio.",
                "first set of layouts": aar1,
                "second set of layouts": aar2
                }

    layoutMetric1 = Metric_Function[metric.name].value(list(layouts1.values()))
    layoutMetric2 = Metric_Function[metric.name].value(list(layouts2.values()))
    betterLayout = "first" if (layoutMetric1 < layoutMetric2) else "second"
    return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser "+ metric.value,
            "first dataset": [x for x in layouts1],
            metric.value+" first dataset": layoutMetric1,
            "second dataset": [x for x in layouts2],
            metric.value+" second dataset": layoutMetric2
            }
    
            
def data_fullreport(layouts):

    aar = [
        {
            "Dataset": i,
            "average aspect ratio": averageAspectRatio.averageAspectRatio(
                layouts[i]
            ),
        }
        for i in layouts
    ]

    rdc = relativeDirectionChange.rdc(list(layouts.values()))

    rdc_ri = relativeDirectionChange.rdc_ri(list(layouts.values()))

    ld = locationDrift.meanLocationDrift(list(layouts.values()))

    return {
        "Dataset": [x for x in layouts],
        "average aspect ratio": aar,
        "relative direction change": rdc,
        "relative direction change rotation invariant": rdc_ri,
        "location drift": ld
    }