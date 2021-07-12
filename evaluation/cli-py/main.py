from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio
import mockdata as md
import pandas as pd
import io

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


@app.post("/uploadfile/{metric}")
async def create_upload_file(metric, file: UploadFile = File(...)):
    content = await file.read()
    dataframe = pd.read_csv(io.StringIO(content.decode()))

    if metric == "average-aspect-ratio":
        aar = averageAspectRatio.averageAspectRatio(dataframe)
        return {
            "Dataset": file.filename,
            "average aspect ratio": aar}

    if metric == "relative-direction-change":
        rdc = relativeDirectionChange.rdc([dataframe])
        return {
            "Dataset": file.filename,
            "relative direction change": rdc}

    if metric == "location-drift":
        ld = locationDrift.meanLocationDrift([dataframe])
        return {
            "Dataset": file.filename,
            "location drift": ld}

    return {"no valid metric selected"}