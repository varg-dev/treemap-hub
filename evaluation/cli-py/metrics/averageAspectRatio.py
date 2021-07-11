import math
import pandas as pd

def averageAspectRatio(layout):
    aspectRatios = [
        max(row['width'] / row['height'], row['height'] / row['width'])
        for index, row in layout.iterrows()
    ]
    return pd.DataFrame(aspectRatios).mean()[0]