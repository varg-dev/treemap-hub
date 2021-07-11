# Location Drift as described in "Enhanced Spatial Stability with Hilbertand Moore Treemaps" by Susanne Tak and Andy Cockburn

import math
import pandas as pd

def centerOfGravity(layouts, maxindex): #calculates the center of gravity for each element
    cog = pd.DataFrame(columns = ['#index', 'center_x', 'center_y'])
    for i in range (maxindex+1):
        values = len(layouts)
        cog_x = 0
        cog_y = 0
        for layout in layouts:
            local_x = layout.loc[layout['#index'] == i]['center_x'].values
            local_y = layout.loc[layout['#index'] == i]['center_y'].values
            if len(local_x) == 0 or len(local_y) == 0:
                values -=1
            else:
                cog_x += local_x[0]
                cog_y += local_y[0]
        if values > 0:
            cog = cog.append(pd.DataFrame([[i, cog_x/values, cog_y/values]], columns = ['#index', 'center_x', 'center_y']))
    return cog

def itemLocationDrift(layouts, maxindex, cog): #calculates the location drift for each element
    ild = pd.DataFrame(columns = ['#index', 'drift'])
    for i in range (maxindex+1):
        values = len(layouts)
        drift = 0
        for layout in layouts:
            local_x = layout.loc[layout['#index'] == i]['center_x'].values
            local_y = layout.loc[layout['#index'] == i]['center_y'].values
            if len(local_x) == 0 or len(local_y) == 0:
                values -=1
            else:
                cog_x = cog.loc[cog['#index'] == i]['center_x'].values
                cog_y = cog.loc[cog['#index'] == i]['center_y'].values
                drift += math.dist((local_x[0], local_y[0]),(cog_x[0], cog_y[0]))
        if values > 0:
            ild = ild.append(pd.DataFrame([[i, drift/values]], columns = ['#index', 'drift']))
    return ild

def meanLocationDrift(layouts): #calculates the mean location drift over the layouts
    maxindex = 0
    for i, layout in enumerate(layouts):
        localmax = layout['#index'].max()
        if (localmax > maxindex):
            maxindex = localmax
    cog = centerOfGravity(layouts, maxindex)
    ild = itemLocationDrift(layouts, maxindex, cog)
    return ild['drift'].mean()