from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio

import pandas as pd
import io
from typing import List
from enum import Enum
from fastapi import File, UploadFile

class Metric(str, Enum):
    aar = "average-aspect-ratio"
    rdc = "relative-direction-change"
    rdc_ri = "relative-direction-change-rotation-invariant"
    ld = "location-drift"

def data_metrics(metric: Metric, layouts):

    if metric == Metric.aar:
        result = []
        for i in layouts:
            aar = averageAspectRatio.averageAspectRatio(layouts[i])
            result.append({
                "Dataset": i,
                "average aspect ratio": aar
            })
        return {str(result)}

    if metric == Metric.rdc:
        rdc = relativeDirectionChange.rdc(list(layouts.values()))
        return {
            "Dataset": [x for x in layouts],
            "relative direction change": rdc}

    if metric == Metric.rdc_ri:
        rdc_ri = relativeDirectionChange.rdc_ri(list(layouts.values()))
        return {
            "Dataset": [x for x in layouts],
            "relative direction change rotation invariant": rdc_ri}

    if metric == Metric.ld:
        ld = locationDrift.meanLocationDrift(list(layouts.values()))
        return {
            "Dataset": [x for x in layouts],
            "location drift": ld}


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

    if metric == Metric.rdc:
        rdc1 = relativeDirectionChange.rdc(list(layouts1.values()))
        rdc2 = relativeDirectionChange.rdc(list(layouts2.values()))
        betterLayout = "first" if (rdc1 < rdc2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser Relative Direction Change",
                "first dataset": [x for x in layouts1],
                "relative direction change first dataset": rdc1,
                "second dataset": [x for x in layouts2],
                "relative direction change second dataset": rdc2
                }

    if metric == Metric.rdc_ri:
        rdc_ri1 = relativeDirectionChange.rdc_ri(list(layouts1.values()))
        rdc_ri2 = relativeDirectionChange.rdc_ri(list(layouts2.values()))
        betterLayout = "first" if (rdc_ri1 < rdc_ri2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser rotation-invariant Relative Direction Change",
                "first dataset": [x for x in layouts1],
                "relative direction change rotation-invariant first dataset": rdc_ri1,
                "second dataset": [x for x in layouts2],
                "relative direction change rotation-invariant second dataset": rdc_ri2
                }

    if metric == Metric.ld:
        ld1 = locationDrift.meanLocationDrift(list(layouts1.values()))
        ld2 = locationDrift.meanLocationDrift(list(layouts2.values()))

        betterLayout = "first" if (ld1 < ld2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser Location Drift",
                "first dataset": [x for x in layouts1],
                "location drift first dataset": ld1,
                "second dataset": [x for x in layouts2],
                "location drift second dataset": ld2
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