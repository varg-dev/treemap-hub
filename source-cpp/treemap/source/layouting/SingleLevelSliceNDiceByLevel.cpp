
#include <treemap/layouting/SingleLevelSliceNDiceByLevel.h>

#include <treemap/layouting/Slice.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


void SingleLevelSliceNDiceByLevel::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout)
{
    auto slice = Slice(tree, weights, rect, weights[parent], parent->depth() % 2 == 0);

    tree->childrenDo(parent, [& slice, & weights](const LinearizedTreeNode * current) {
        slice.insert(current, weights[current]);
    });

    slice.layoutNodes(layout);
}
