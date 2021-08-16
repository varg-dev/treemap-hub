# Relative Direction Change as described in "Relative Direction Change – A Topology-based Metric for Layout Stability in Treemaps" by Sebastian Hahn, Joseph Bethge and Jürgen Döllner

import math

def matrixvalue(layout, i, j):
    return math.atan2(
                float(layout.loc[layout['#index'] == j].center_y)
                    -float(layout.loc[layout['#index'] == i].center_y), 
                float(layout.loc[layout['#index'] == j].center_x)
                    -float(layout.loc[layout['#index'] == i].center_x))

def valuemissing(layout1, layout2, i, j):
    return len(layout1.loc[layout1['#index'] == j]) == 0 \
            or len(layout1.loc[layout1['#index'] == i]) == 0 \
            or len(layout2.loc[layout2['#index'] == j]) == 0 \
            or len(layout2.loc[layout2['#index'] == i]) == 0

def _rdc(layout1, layout2): #Relative Direction Change of two layouts
    n = max(len(layout1), len(layout2))
    sumOfRowAgvs = 0
    for i in range(n):
        rowSum = 0
        for j in range (n):
            if valuemissing(layout1, layout2, i, j):
                n -= 1
            elif i != j:
                rowSum += pow(matrixvalue(layout1, i, j)-matrixvalue(layout2, i, j), 2)
        rowAvg = math.sqrt(rowSum)/(n-1)
        sumOfRowAgvs += rowAvg
    return sumOfRowAgvs/n

def _rdc_ri(layout1, layout2): #Relative Direction Change Rotation-Invariant of two layouts
    n = max(len(layout1), len(layout2))
    actual_n = n
    outerSum = 0
    for i in range(n):
        rowSum = 0
        for j in range (n):
            if valuemissing(layout1, layout2, i, j):
                actual_n -= 1
            elif i != j:
                rowSum += pow(matrixvalue(layout1, i, j)-matrixvalue(layout2, i, j), 2)
        rowAvg = math.sqrt(rowSum)/(actual_n-1)

        innerSum = sum(
            abs(
                abs(matrixvalue(layout1, i, j) - matrixvalue(layout2, i, j))
                - rowAvg
            )
            for j in range(n)
            if (not valuemissing(layout1, layout2, i, j) and i != j)
        )

        outerSum += innerSum
    return outerSum/(actual_n*actual_n-actual_n)

def rdc(layouts): # relative direciton change of two or more layouts in a sorted array
    rdcsum = sum(_rdc(layouts[i], layouts[i+1]) for i in range (len(layouts)-1))
    return rdcsum/(len(layouts)-1)

def rdc_ri(layouts): # relative direciton change rotation-invariant of two or more layouts in a sorted array
    rdcrisum = sum(_rdc_ri(layouts[i], layouts[i + 1]) for i in range(len(layouts) - 1))
    return rdcrisum/(len(layouts)-1)