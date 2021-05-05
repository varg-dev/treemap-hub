
#pragma once


/**
 * @brief The RectangularLayoutAlgorithm enum
 *
 * Supported:
 * - SliceNDice
 * - SliceNDice_ByAspectRatio
 * - Squarified
 * - Pivot_BySize
 * - Pivot_ByMiddle
 * - Pivot_BySplitSize
 * - Strip
 * - Strip_Inverted
 * - Spiral
 * - Approximation
 * - Moore_Greedy
 * - Moore_MinMax
 * - Moore_MinVariance
 * - Hilbert_Greedy
 * - Hilbert_MinMax
 * - Hilbert_MinVariance
 *
 * TODO:
 * - 1D Partitioning, 1993
 * - 2D Partitioning, 1993
 * - Interactive Dynamic Map, 1994
 * - Cluster, 1999
 * - Modifiable, 2000
 * - BinaryTree, 2001
 * - RecMap V1, 2004
 * - EncCon, 2005
 * - Matrix, 2006
 * - Snake, Spiral, 2007
 * - Document Page Fit, 2008
 * - Approximation by Impact, 2014
 * - Nmap, 2014
 * - Squarified+, 2016
 * - Golden Rectangle, 2017
 * - Greedy Insertion, 2018
 * - Balanced Partitioning (Size-balanced, Sequence-balanced, Number-balanced), 2019
 *
 * Check:
 * - ID-Map, 2005
 * - Deterministic Hierarchical Clustering, 2007
 * - Density-guided, 2008
 * - iMap, 2013
 * - Template-based Splitting, 2014
 */
enum class RectangularLayoutAlgorithm : unsigned int
{
    // Slice'n'Dice
    SliceNDice
,   SliceNDice_ByAspectRatio

    // Squarified
,   Squarified

    // Pivot
,   Pivot_BySize
,   Pivot_ByMiddle
,   Pivot_BySplitSize

    // Strip
,   Strip
,   Strip_Inverted

    // Spiral
,   Spiral

    // Approximation
,   Approximation

    // Hilbert & Moore
,   Moore_Greedy
,   Moore_MinMax
,   Moore_MinVariance
,   Hilbert_Greedy
,   Hilbert_MinMax
,   Hilbert_MinVariance
};
