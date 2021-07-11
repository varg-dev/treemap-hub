from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio
import mockdata as md

from fastapi import FastAPI

app = FastAPI()

@app.get("/{metric}")
async def read_item(metric):

    if metric == "average-aspect-ratio":
        mockdata = md.generateMockdata()
        aar = averageAspectRatio.averageAspectRatio(mockdata[0])
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "average aspect ratio 2012": aar}

    if metric == "relative-direction-change":
        mockdata = md.generateMockdata()
        rdc = relativeDirectionChange.rdc(mockdata)
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "relative direction change 2012-2013-2014": rdc}

    if metric == "location-drift":
        mockdata = md.generateMockdata()
        ld = locationDrift.meanLocationDrift(mockdata)
        return {
            "Dataset": "names-layouts/popular-names-hilbert-greedy-false-false-0.6",
            "location drift 2012-2013-2014": ld}

    return {"no valid metric selected"}