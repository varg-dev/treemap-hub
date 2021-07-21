from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio
import mockdata as md
import pandas as pd
import io
from typing import List

from fastapi import FastAPI, File, UploadFile

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


@app.post("/uploadfiles/{metric}")
async def create_multiple_files(metric, files: List[UploadFile] = File(...)):
    layouts = []
    for file in files:
        content = await file.read()
        layout = pd.read_csv(io.StringIO(content.decode()))
        layout['center_x']=layout['left']+(layout['width']/2)                   #ggf anpassen um verschiedene Formate abzudecken
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts.append(layout)

    if metric == "average-aspect-ratio":
        result = []
        for i in range(len(layouts)):
            aar = averageAspectRatio.averageAspectRatio(layouts[i])
            result.append({
                "Dataset": files[i].filename,
                "average aspect ratio": aar
            })
        return {str(result)}

    if metric == "relative-direction-change":
        rdc = relativeDirectionChange.rdc(layouts)
        return {
            "Dataset": [file.filename for file in files],
            "relative direction change": rdc}

    if metric == "location-drift":
        ld = locationDrift.meanLocationDrift(layouts)
        return {
            "Dataset": [file.filename for file in files],
            "location drift": ld}
    
    return {"files": len(layouts)}
