# Relative Direction Change as described in "Relative Direction Change – A Topology-based Metric for Layout Stability in Treemaps" by Sebastian Hahn, Joseph Bethge and Jürgen Döllner

import math
import pandas as pd
import numpy as np

# Is "#index" always integer? -> but if not a mapping can be done in preprocessing

#gets Dataframe that at least has the columns "#index", "center_x", "center_y"
def fillmatrix(layout):
    matrix = np.zeros( (layout['#index'].max()+1, layout['#index'].max()+1) )
    #If a index is not existent the value in the matrix there is zero. is that correct or does that destroy something? what to do with disappearing indices?
    # -> schauen ob knoten in beidem drin ist, wenn nicht: nicht mit berechnen (auch n entsprechend reduzieren)
    for index, startrow in layout.iterrows():
        for index,endrow in layout.iterrows():
            temp = math.atan2(float(endrow['center_y'])-float(startrow['center_y']), float(endrow['center_x'])-float(startrow['center_x']))
            matrix[int(startrow['#index'])][int(endrow['#index'])] = temp
    return matrix

def rowAVG(i, matrix1, matrix2):
    n = matrix1.shape[1]
    avg = sum(pow(matrix1[i][j]-matrix2[i][j], 2) for j in range (n) if i != j)
    avg = math.sqrt(avg)/(n-1)
    return avg

def _rdc(layout1, layout2): #Relative Direction Change of two layouts
    n = max(len(layout1), len(layout2))
    thisrdc = 0
    for i in range(n):
        temp = 0
        for j in range (n):
            if len(layout1.loc[layout1['#index'] == j]) == 0 \
                or len(layout1.loc[layout1['#index'] == i]) == 0 \
                or len(layout2.loc[layout2['#index'] == j]) == 0 \
                or len(layout2.loc[layout2['#index'] == i]) == 0:
                n -= 1
            elif i != j:
                matrix1ij = math.atan2(
                                float(layout1.loc[layout1['#index'] == j].center_y)
                                    -float(layout1.loc[layout1['#index'] == i].center_y), 
                                float(layout1.loc[layout1['#index'] == j].center_x)
                                    -float(layout1.loc[layout1['#index'] == i].center_x))
                matrix2ij = math.atan2(
                                float(layout2.loc[layout2['#index'] == j].center_y)
                                    -float(layout2.loc[layout2['#index'] == i].center_y), 
                                float(layout2.loc[layout2['#index'] == j].center_x)
                                    -float(layout2.loc[layout2['#index'] == i].center_x))
                temp += pow(matrix1ij-matrix2ij, 2)
        thisrdc += math.sqrt(temp)/(n-1)
    return thisrdc/n

def _oldrdc(layout1, layout2): #Relative Direction Change of two layouts
    matrix1 = fillmatrix(layout1)
    matrix2 = fillmatrix(layout2) #matrix nicht speichern sondern berechnung inline bauen
    n = matrix1.shape[0] #probably max from the matrixes? What happens if they dont have the same size?
    rdc = sum(rowAVG(i, matrix1, matrix2) for i in range(n))
    return rdc/n

def _rdc_ri(layout1, layout2): #Relative Direction Change Rotation-Invariant of two layouts
    matrix1 = fillmatrix(layout1)
    matrix2 = fillmatrix(layout2) #matrix nicht speichern sondern berechnung inline bauen
    rdcri = 0
    n = matrix1.shape[0]
    for i in range (n):
        avg = 0
        for j in range (n):
            if not i==j:
                avg += abs(abs(matrix1[i][j]-matrix2[i][j])-rowAVG(i, matrix1, matrix2))
        rdcri += avg
    rdcri = rdcri/(n*n-n)
    return rdcri

def rdc(layouts): # relative direciton change of two or more layouts in a sorted array
    rdcsum = sum(_rdc(layouts[i], layouts[i+1]) for i in range (len(layouts)-1))
    return rdcsum/(len(layouts)-1)

def rdc_ri(layouts): # relative direciton change rotation-invariant of two or more layouts in a sorted array
    rdcrisum = sum(_rdc_ri(layouts[i], layouts[i + 1]) for i in range(len(layouts) - 1))
    return rdcrisum/(len(layouts)-1)