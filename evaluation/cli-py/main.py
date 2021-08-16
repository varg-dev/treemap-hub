from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio
import mockdata as md
import pandas as pd
import io
from typing import List
from enum import Enum

from fastapi import FastAPI, File, UploadFile

class Metric(str, Enum):
    aar = "average-aspect-ratio"
    rdc = "relative-direction-change"
    rdc_ri = "relative-direction-change-rotation-invariant"
    ld = "location-drift"

app = FastAPI()

@app.get("/{metric}")
async def mockdata_metrics(metric: Metric):

    if metric == Metric.aar:
        mockdata = md.generateMockdata()
        aar = averageAspectRatio.averageAspectRatio(mockdata[0])
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "average aspect ratio 2012": aar}

    if metric == Metric.rdc:
        mockdata = md.generateMockdata()
        rdc = relativeDirectionChange.rdc(mockdata)
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "relative direction change 2012-2013-2014": rdc}
    
    if metric == Metric.rdc_ri:
        mockdata = md.generateMockdata()
        rdc_ri = relativeDirectionChange.rdc_ri(mockdata)
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "relative direction change rotation invariant 2012-2013-2014": rdc_ri}

    if metric == Metric.ld:
        mockdata = md.generateMockdata()
        ld = locationDrift.meanLocationDrift(mockdata)
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "location drift 2012-2013-2014": ld}

@app.post("/uploadfiles/{metric}")
async def data_metrics(metric: Metric, files: List[UploadFile] = File(...)):
    layouts = []
    for file in files:
        content = await file.read()
        layout = pd.read_csv(io.StringIO(content.decode()))
        layout['center_x']=layout['left']+(layout['width']/2)                   #ggf anpassen um verschiedene Formate abzudecken
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts.append(layout)

    if metric == Metric.aar:
        result = []
        for i in range(len(layouts)):
            aar = averageAspectRatio.averageAspectRatio(layouts[i])
            result.append({
                "Dataset": files[i].filename,
                "average aspect ratio": aar
            })
        return {str(result)}

    if metric == Metric.rdc:
        rdc = relativeDirectionChange.rdc(layouts)
        return {
            "Dataset": [file.filename for file in files],
            "relative direction change": rdc}

    if metric == Metric.rdc_ri:
        rdc_ri = relativeDirectionChange.rdc_ri(layouts)
        return {
            "Dataset": [file.filename for file in files],
            "relative direction change rotation invariant": rdc_ri}

    if metric == Metric.ld:
        ld = locationDrift.meanLocationDrift(layouts)
        return {
            "Dataset": [file.filename for file in files],
            "location drift": ld}

@app.post("/comparison/{metric}")
async def data_comparison(metric: Metric, files1: List[UploadFile] = File(...), files2: List[UploadFile] = File(...)):
    layouts1 = []
    layouts2 = []
    for file in files1:
        content = await file.read()
        layout = pd.read_csv(io.StringIO(content.decode()))
        layout['center_x']=layout['left']+(layout['width']/2)
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts1.append(layout)
    for file in files2:
        content = await file.read()
        layout = pd.read_csv(io.StringIO(content.decode()))
        layout['center_x']=layout['left']+(layout['width']/2)
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts2.append(layout)

    if metric == Metric.aar:
        aar1 = []
        aar2 = []

        avgAar1 = 0
        for i in range(len(layouts1)):
            aar = averageAspectRatio.averageAspectRatio(layouts1[i])
            aar1.append({
                "Dataset": files1[i].filename,
                "average aspect ratio": aar
            })
            avgAar1 += aar
        avgAar1 /= len(layouts1)

        avgAar2 = 0
        for i in range(len(layouts2)):
            aar = averageAspectRatio.averageAspectRatio(layouts2[i])
            aar2.append({
                "Dataset": files2[i].filename,
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
        rdc1 = relativeDirectionChange.rdc(layouts1)
        rdc2 = relativeDirectionChange.rdc(layouts2)
        betterLayout = "first" if (rdc1 < rdc2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser Relative Direction Change",
                "first dataset": [file.filename for file in files1],
                "relative direction change first dataset": rdc1,
                "second dataset": [file.filename for file in files2],
                "relative direction change second dataset": rdc2
                }

    if metric == Metric.rdc_ri:
        rdc_ri1 = relativeDirectionChange.rdc_ri(layouts1)
        rdc_ri2 = relativeDirectionChange.rdc_ri(layouts2)
        betterLayout = "first" if (rdc_ri1 < rdc_ri2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser rotation-invariant Relative Direction Change",
                "first dataset": [file.filename for file in files1],
                "relative direction change rotation-invariant first dataset": rdc_ri1,
                "second dataset": [file.filename for file in files2],
                "relative direction change rotation-invariant second dataset": rdc_ri2
                }

    if metric == Metric.ld:
        ld1 = locationDrift.meanLocationDrift(layouts1)
        ld2 = locationDrift.meanLocationDrift(layouts2)

        betterLayout = "first" if (ld1 < ld2) else "second"
        return {"result": "The layouts from the " + betterLayout + " set of layouts have the lesser Location Drift",
                "first dataset": [file.filename for file in files1],
                "location drift first dataset": ld1,
                "second dataset": [file.filename for file in files2],
                "location drift second dataset": ld2
                }

@app.post("/fullreport")
async def data_fullreport(files: List[UploadFile] = File(...)):
    layouts = []
    for file in files:
        content = await file.read()
        layout = pd.read_csv(io.StringIO(content.decode()))
        layout['center_x']=layout['left']+(layout['width']/2)                   #ggf anpassen um verschiedene Formate abzudecken
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts.append(layout)

    aar = [
        {
            "Dataset": files[i].filename,
            "average aspect ratio": averageAspectRatio.averageAspectRatio(
                layouts[i]
            ),
        }
        for i in range(len(layouts))
    ]

    rdc = relativeDirectionChange.rdc(layouts)

    rdc_ri = relativeDirectionChange.rdc_ri(layouts)

    ld = locationDrift.meanLocationDrift(layouts)

    return {
        "Dataset": [file.filename for file in files],
        "average aspect ratio": aar,
        "relative direction change": rdc,
        "relative direction change rotation invariant": rdc_ri,
        "location drift": ld
    }