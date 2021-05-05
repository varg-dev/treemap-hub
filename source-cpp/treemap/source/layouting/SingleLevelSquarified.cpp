
#include <treemap/layouting/SingleLevelSquarified.h>

#include <treemap/layouting/Row.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/linearizedtree/LinearizedTreeNode.h>


void SingleLevelSquarified::layout(const LinearizedTreeNode * parent, const Rect & rect, const LinearizedTree * tree, const LinearizedBuffer<float> & weights, LinearizedBuffer<Rect> & layout)
{
    auto currentRow = Row(tree, weights, rect, weights[parent]);

    tree->childrenDo(parent, [& currentRow, &layout, & weights](const LinearizedTreeNode * current) {
        const auto weight = weights[current];

        if (currentRow.increasesWorstAspectRatio(weight))
        {
            currentRow.layoutNodes(layout);

            currentRow.next();
        }

        currentRow.insert(current, weight);
    });

    currentRow.layoutNodes(layout);
}
