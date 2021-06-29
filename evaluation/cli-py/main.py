import pandas as pd
from metrics import relativeDirectionChange
from metrics import locationDrift
from metrics import averageAspectRatio

def readerForMockdata(path):
    layout = pd.read_csv(filepath_or_buffer=path, delimiter=',')
    layout['center_x']=layout['left']+(layout['width']/2)
    layout['center_y']=layout['bottom']+(layout['height']/2)
    return layout

layout2012 = readerForMockdata('../../datasets/names-layouts/popular-names-hilbert-greedy-false-false-0.6/2012.csv')
layout2013 = readerForMockdata('../../datasets/names-layouts/popular-names-hilbert-greedy-false-false-0.6/2013.csv')
layout2014 = readerForMockdata('../../datasets/names-layouts/popular-names-hilbert-greedy-false-false-0.6/2014.csv')

layouts = [layout2012, layout2013, layout2014]

print("relative direction change 2012-2013-2014: ", relativeDirectionChange.rdc(layouts))
print("location drift 2012-2013-2014: ", locationDrift.meanLocationDrift(layouts))
print("average aspect ratio 2012: ", averageAspectRatio.averageAspectRatio(layout2012))