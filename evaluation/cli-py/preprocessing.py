import pandas as pd
import io
from typing import List
from fastapi import FastAPI, File, UploadFile
from pathlib import Path

def layoutsFromFilepaths(paths: List[Path]):
    layouts = {}
    tempfolder = paths
    if paths[0].is_dir:
        tempfolder = Path(paths[0]).rglob('*.csv') #ToDo: sort files
    for file in tempfolder:         
        layout = pd.read_csv(file)
        layout['center_x']=layout['left']+(layout['width']/2)
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts[file.name]=layout 
    return layouts

async def layoutsFromFiles (files: List[UploadFile] = File(...)):
    layouts = {}
    for file in files:
        content = await file.read()
        layout = pd.read_csv(io.StringIO(content.decode()))
        layout['center_x']=layout['left']+(layout['width']/2)
        layout['center_y']=layout['bottom']+(layout['height']/2)
        layouts[file.filename]=layout
    return layouts