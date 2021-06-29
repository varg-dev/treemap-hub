import math
import pandas as pd

def averageAspectRatio(layout):
    aspectRatios = []
    for index, row in layout.iterrows():
        aspectRatios.append(max(row['width']/row['height'], row['height']/row['width']))
    return pd.DataFrame(aspectRatios).mean()[0]