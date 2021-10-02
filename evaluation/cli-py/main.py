import mockdata as md
import metricCalculation as mc
import preprocessing as prep
from typing import List
from enum import Enum
import asyncio

import shutil
import os
from pathlib import Path
from tempfile import NamedTemporaryFile
from zipfile import ZipFile

from fastapi import FastAPI, File, UploadFile

datasetDir = os.environ.get('DATASET_DIRECTORY', '/tmp/treemap-hub')

class Metric(str, Enum):
    aar = "average-aspect-ratio"
    rdc = "relative-direction-change"
    rdc_ri = "relative-direction-change-rotation-invariant"
    ld = "location-drift"

app = FastAPI()

@app.get("/{metric}")
async def mockdata_metrics(metric: Metric):
    mockdata = md.generateMockdata()
    return mc.data_metrics(metric, mockdata)

@app.post("/uploadfiles/metric/{metric}")
async def data_metrics(metric: Metric, files: List[UploadFile] = File(...)):
    layouts = await prep.layoutsFromFiles(files)
    return mc.data_metrics(metric, layouts)

@app.post("/pathlist/metric/{metric}")
async def data_metrics(metric: Metric, paths: List[Path]):
    layouts = prep.layoutsFromFilepaths(paths)
    return mc.data_metrics(metric, layouts)

@app.post("/uploadfile/comparison/{metric}")
async def data_comparison(metric: Metric, files1: List[UploadFile] = File(...), files2: List[UploadFile] = File(...)):
    layouts1, layouts2 = await asyncio.gather(prep.layoutsFromFiles(files1), prep.layoutsFromFiles(files2))
    return mc.data_comparison(metric, layouts1, layouts2)

@app.post("/pathlist/comparison/{metric}")
async def data_comparison(metric: Metric, paths1: List[Path], paths2: List[Path]):
    layouts1 = prep.layoutsFromFilepaths(paths1)
    layouts2 = prep.layoutsFromFilepaths(paths2)
    return mc.data_comparison(metric, layouts1, layouts2)

@app.post("/uploadfile/fullreport")
async def data_fullreport(files: List[UploadFile] = File(...)):
    layouts = await prep.layoutsFromFiles(files)
    return mc.data_fullreport(layouts)

@app.post("/pathlist/fullreport")
async def data_fullreport(paths: List[Path]):
    layouts = prep.layoutsFromFilepaths(paths)
    return mc.data_fullreport(layouts)

@app.post("/store/upload")
async def save_upload_file_in_store(upload_file: UploadFile = File(...)) -> Path:
    try:
        suffix = Path(upload_file.filename).suffix
        prefix = Path(upload_file.filename).stem + "_"
        
        with NamedTemporaryFile(delete=False, prefix=prefix, suffix=suffix, dir=datasetDir) as tmp:    #ToDo: Ordner dafür automatisch anlegen lassen oder als part vom projekt oder so
            shutil.copyfileobj(upload_file.file, tmp)
            tmp_path = Path(tmp.name)
            if suffix == ".zip":
                with ZipFile(tmp_path, 'r') as zip: 
                    zip.extractall(datasetDir)
    finally:
        upload_file.file.close()
    return tmp_path

@app.post("/store/delete/")
async def delete_file_from_store(path: Path):
    if (path.is_file()):
        os.remove(path)
        return "sucessfully removed file"
    if (path.is_dir()):
        shutil.rmtree(path)
        return "sucessfully removed directory"
    return {}

@app.get("/store/list") 
async def get_list_of_store():
    tempfolder = Path(datasetDir).glob('*')
    return [x for x in tempfolder]