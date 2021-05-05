
#include <treemap/layouting/TreemapLayouter.h>

#include <treemap/layout/Rect.h>
#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/layouting/SingleLevelApproximation.h>
#include <treemap/layouting/SingleLevelHilbertMoore.h>
#include <treemap/layouting/SingleLevelPivot.h>
#include <treemap/layouting/SingleLevelSliceNDice.h>
#include <treemap/layouting/SingleLevelSliceNDiceByLevel.h>
#include <treemap/layouting/SingleLevelSpiral.h>
#include <treemap/layouting/SingleLevelSquarified.h>
#include <treemap/layouting/SingleLevelStrip.h>
#include <treemap/layouting/SingleLevelStripInverted.h>


namespace
{


const auto rootRect = Rect(0.0f, 0.0f, 1.0f, 1.0f);


}


TreemapLayouter::TreemapLayouter()
: m_algorithm(RectangularLayoutAlgorithm::Strip)
{
}

TreemapLayouter::~TreemapLayouter()
{
}

void TreemapLayouter::initialize()
{
}

void TreemapLayouter::deinitialize()
{
}

void TreemapLayouter::setAlgorithm(RectangularLayoutAlgorithm algorithm)
{
    m_algorithm = algorithm;
}

void TreemapLayouter::process(const LinearizedTree * tree, const LinearizedBuffer<float> & weights,
    LinearizedBuffer<Rect> & layout)
{
    layout[tree->root()] = rootRect;

    if (tree->size() <= 1)
    {
        return;
    }

    // Weight prefix sum, double precision not enough for mid-sized datasets
    // LinearizedBuffer<double> prefixSum(std::vector<double>(weights.size()));
    // std::partial_sum(weights.begin(), weights.end(), prefixSum.begin());

    tree->parentsDo([this, &tree, &weights, &layout](const LinearizedTreeNode * parent) {
        singleNodeProcess(tree, parent, weights, layout);
    });
}

void TreemapLayouter::singleNodeProcess(const LinearizedTree *tree, const LinearizedTreeNode * parent, const LinearizedBuffer<float> &weights, LinearizedBuffer<Rect> &layout)
{
    const auto & workspace = layout[parent];

    switch (m_algorithm)
    {
    case RectangularLayoutAlgorithm::SliceNDice:
        SingleLevelSliceNDiceByLevel::layout(parent, workspace, tree, weights, layout);
        break;
    case RectangularLayoutAlgorithm::SliceNDice_ByAspectRatio:
        SingleLevelSliceNDice::layout(parent, workspace, tree, weights, layout);
        break;

    // Squarified
    case RectangularLayoutAlgorithm::Squarified:
        SingleLevelSquarified::layout(parent, workspace, tree, weights, layout);
        break;

    // Pivot
    case RectangularLayoutAlgorithm::Pivot_BySize:
        SingleLevelPivot::layout(parent, workspace, tree, weights, layout, pivotAlgorithms::pivotBySize);
        break;
    case RectangularLayoutAlgorithm::Pivot_ByMiddle:
        SingleLevelPivot::layout(parent, workspace, tree, weights, layout, pivotAlgorithms::pivotByMiddle);
        break;
    case RectangularLayoutAlgorithm::Pivot_BySplitSize:
        SingleLevelPivot::layout(parent, workspace, tree, weights, layout, pivotAlgorithms::pivotBySplitSize);
        break;

    // Strip
    case RectangularLayoutAlgorithm::Strip:
        SingleLevelStrip::layout(parent, workspace, tree, weights, layout);
        break;
    case RectangularLayoutAlgorithm::Strip_Inverted:
        SingleLevelStripInverted::layout(parent, workspace, tree, weights, layout);
        break;

    // Spiral
    case RectangularLayoutAlgorithm::Spiral:
        SingleLevelSpiral::layout(parent, workspace, tree, weights, layout);
        break;

    // Approximation
    case RectangularLayoutAlgorithm::Approximation:
        SingleLevelApproximation::layout(parent, workspace, tree, weights, layout);
        break;

    // Hilbert & Moore
    case RectangularLayoutAlgorithm::Moore_Greedy:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::Greedy);
        break;
    case RectangularLayoutAlgorithm::Moore_MinMax:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::MinMax);
        break;
    case RectangularLayoutAlgorithm::Moore_MinVariance:
        SingleLevelHilbertMoore::layoutMoore(parent, workspace, tree, weights, layout, PartitionAlg::Variance);
        break;
    case RectangularLayoutAlgorithm::Hilbert_Greedy:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::Greedy);
        break;
    case RectangularLayoutAlgorithm::Hilbert_MinMax:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::MinMax);
        break;
    case RectangularLayoutAlgorithm::Hilbert_MinVariance:
        SingleLevelHilbertMoore::layoutHilbert(parent, workspace, tree, weights, layout, PartitionAlg::Variance);
        break;

    default:
        break;
    }
}
