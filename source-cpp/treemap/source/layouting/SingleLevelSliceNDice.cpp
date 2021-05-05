
#include <treemap/layouting/SingleLevelSliceNDice.h>

#include <treemap/layouting/Slice.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


void SingleLevelSliceNDice::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout)
{
    SingleLevelSliceNDice::layout(parent, rect, tree, weights, layout, rect.isHorizontal());
}

void SingleLevelSliceNDice::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, bool horizontal)
{
    auto slice = Slice(tree, weights, rect, weights[parent], horizontal);

    tree->childrenDo(parent, [& slice, & weights](const LinearizedTreeNode * current) {
        slice.insert(current, weights[current]);
    });

    slice.layoutNodes(layout);
}
