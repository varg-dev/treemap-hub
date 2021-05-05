
#include <treemap/layouting/SingleLevelStripInverted.h>

#include <treemap/layouting/DirectionalRow.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


void SingleLevelStripInverted::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout, bool optimized)
{
    const auto horizontal = rect.isVertical();

    auto currentRow = DirectionalRow(tree, weights, rect, weights[parent], horizontal);

    tree->childrenDo(parent, [& currentRow, & horizontal, & optimized, &layout, & weights](const LinearizedTreeNode * current) {
        const auto weight = weights[current];

        if (currentRow.increasesAverageAspectRatio(weight, optimized))
        {
            currentRow.layoutNodes(layout);

            currentRow.next(horizontal);
        }

        currentRow.insert(current, weight);
    });

    currentRow.layoutNodes(layout);
}
