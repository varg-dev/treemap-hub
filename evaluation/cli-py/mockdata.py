import pandas as pd

def readerForMockdata(path):
    layout = pd.read_csv(filepath_or_buffer=path, delimiter=',')
    layout['center_x']=layout['left']+(layout['width']/2)
    layout['center_y']=layout['bottom']+(layout['height']/2)
    return layout

def generateMockdata():
    layout2012 = readerForMockdata('../../datasets/names-layouts/popular-names-hilbert-greedy-false-false-0.6/2012.csv')
    layout2013 = readerForMockdata('../../datasets/names-layouts/popular-names-hilbert-greedy-false-false-0.6/2013.csv')
    layout2014 = readerForMockdata('../../datasets/names-layouts/popular-names-hilbert-greedy-false-false-0.6/2014.csv')
    return {
        'layout2012': layout2012, 
        'layout2013': layout2013, 
        'layout2014': layout2014}