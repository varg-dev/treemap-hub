import math
import pandas as pd
import numpy as np

# Is "#index" always integer? -> but if not a mapping can be done in preprocessing

#gets Dataframe that at least has the columns "#index", "center_x", "center_y"
def fillmatrix(layout):
    matrix = np.zeros( (layout['#index'].max()+1, layout['#index'].max()+1) )
    #If a index is not existent the value in the matrix there is zero. is that correct or does that destroy something? what to do with disappearing indices?
    for index, startrow in layout.iterrows():
        for index,endrow in layout.iterrows():
            temp = math.atan2(float(endrow['center_y'])-float(startrow['center_y']), float(endrow['center_x'])-float(startrow['center_x']))
            matrix[int(startrow['#index'])][int(endrow['#index'])] = temp
    return matrix

def rowAVG(i, matrix1, matrix2):
    avg = 0
    n = matrix1.shape[1]
    for j in range (0,n):
        if not i==j:
            avg += abs(matrix1[i][j]-matrix2[i][j])
    avg = avg/(n-1)
    return avg

def _rdc(layout1, layout2): #Relative Direction Change of two layouts
    matrix1 = fillmatrix(layout1)
    matrix2 = fillmatrix(layout2)
    rdc= 0
    n = matrix1.shape[0] #probably max from the matrixes? What happens if they dont have the same size?
    for i in range (0,n):
        rdc += rowAVG(i, matrix1, matrix2)
    rdc = rdc/n
    return rdc

def rdc_ri(layout1, layout2): #Relative Direction Change Rotation-Invariant
    matrix1 = fillmatrix(layout1)
    matrix2 = fillmatrix(layout2)
    rdcri = 0
    n = matrix1.shape[0]
    for i in range (0,n):
        avg = 0
        for j in range (0,n):
            if not i==j:
                avg += abs(abs(matrix1[i][j]-matrix2[i][j])-rowAVG(i, matrix1, matrix2))
        rdcri += avg
    rdcri = rdcri/(n*n-n)
    return rdcri

def rdc(layouts): # relative direciton change of two or more layouts in a sorted array
    rdcsum = 0
    for i in range (len(layouts)-1):
        rdcsum += _rdc(layouts[i], layouts[i+1])
    return rdcsum/(len(layouts)-1)