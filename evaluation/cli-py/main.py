import mockdata as md
import metricCalculation as mc
import preprocessing as prep
from typing import List
import asyncio

from metricCalculation import Metric

import shutil
import os
from pathlib import Path
from tempfile import NamedTemporaryFile
from zipfile import ZipFile

from fastapi import FastAPI, File, UploadFile

datasetDir = os.environ.get('DATASET_DIRECTORY', '/tmp/treemap-hub')


app = FastAPI()

@app.get("/mockdata/{metric}")
async def mockdata_metrics(metric: Metric):
    mockdata = md.generateMockdata()
    return mc.data_metrics(metric, mockdata)

@app.post("/metric/uploadfiles/{metric}")
async def metric_for_layout_by_files(metric: Metric, files: List[UploadFile] = File(...)):
    layouts = await prep.layoutsFromFiles(files)
    return mc.data_metrics(metric, layouts)

@app.post("/metric/pathlist/{metric}")
async def metric_for_layout_by_paths(metric: Metric, paths: List[Path]):
    layouts = prep.layoutsFromFilepaths(paths)
    return mc.data_metrics(metric, layouts)

@app.post("/comparison/uploadfile/{metric}")
async def comparison_of_layouts_by_files(metric: Metric, files1: List[UploadFile] = File(...), files2: List[UploadFile] = File(...)):
    layouts1, layouts2 = await asyncio.gather(prep.layoutsFromFiles(files1), prep.layoutsFromFiles(files2))
    return mc.data_comparison(metric, layouts1, layouts2)

@app.post("/comparison/pathlist/{metric}")
async def comparison_of_layouts_by_paths(metric: Metric, paths1: List[Path], paths2: List[Path]):
    layouts1 = prep.layoutsFromFilepaths(paths1)
    layouts2 = prep.layoutsFromFilepaths(paths2)
    return mc.data_comparison(metric, layouts1, layouts2)

@app.post("/fullreport/uploadfile")
async def full_report_of_layouts_by_files(files: List[UploadFile] = File(...)):
    layouts = await prep.layoutsFromFiles(files)
    return mc.data_fullreport(layouts)

@app.post("/fullreport/pathlist")
async def full_report_of_layouts_by_paths(paths: List[Path]):
    layouts = prep.layoutsFromFilepaths(paths)
    return mc.data_fullreport(layouts)

@app.put("/store")
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

@app.delete("/store")
async def delete_file_from_store(path: Path):
    if (path.is_file()):
        os.remove(path)
        return "sucessfully removed file"
    if (path.is_dir()):
        shutil.rmtree(path)
        return "sucessfully removed directory"
    return {}

@app.get("/store") 
async def get_list_of_store():
    tempfolder = Path(datasetDir).glob('*')
    return [x for x in tempfolder]