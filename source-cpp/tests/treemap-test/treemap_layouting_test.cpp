
#include <gmock/gmock.h>

#include <numeric>

#include <treemap/loading/LinearizedCSVImporter.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/preprocessing/AttributeSumAggregation.h>
#include <treemap/preprocessing/TreeProcessing.h>

#include <treemap/layouting/TreemapLayouter.h>
#include <treemap/layout/Rect.h>


class treemap_layouting_test : public testing::Test
{
public:
    treemap_layouting_test()
    {
        LinearizedCSVImporter linearizedImporter;
        const std::string filename1 = "../../datasets/glbinding-former.csv";
        const std::string weightAttribute = "RLoc";

        tree1.reset(linearizedImporter.load(filename1, weightAttribute, nullptr));

        tree2.reset(TreeProcessing().removeZeroWeightNodes(tree1.get(), weightAttribute, nullptr));

        AttributeSumAggregation aggregation;

        weight_values = &tree2->attributeValues(weightAttribute);

        aggregation.process(tree2.get(), *weight_values);

        layout = std::vector<Rect>(tree2->size());
    }


protected:
    LinearizedBuffer<float> * weight_values = nullptr;
    std::unique_ptr<LinearizedTree> tree1 = nullptr;
    std::unique_ptr<LinearizedTree> tree2 = nullptr;
    LinearizedBuffer<Rect> layout;
    TreemapLayouter layouter;


protected:
    void ASSERT_LAYOUT_PROPERTIES(const LinearizedTree * tree, const LinearizedBuffer<Rect> & layout, const LinearizedBuffer<float> & weight_values)
    {
        tree->nodesDo([& layout, & weight_values](const LinearizedTreeNode * node) {
            const auto weight = weight_values[node];

            if (weight < std::numeric_limits<float>::epsilon())
            {
                return;
            }

            const auto & rect = layout[node];
            const auto weight_ratio = weight / weight_values[0];
            const auto area_ratio = rect.area() / layout[0].area();

            ASSERT_NEAR(weight_ratio, area_ratio, 0.0000001f);

            if (node->parent() != static_cast<std::uint32_t>(-1))
            {
                const auto & parentRect = layout[node->parent()];

                ASSERT_TRUE(parentRect.contains(rect));
            }
        });
    }
};


TEST_F(treemap_layouting_test, test_algorithm_slice_n_dice)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::SliceNDice);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_slice_n_dice_by_ar)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::SliceNDice_ByAspectRatio);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_squarified)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Squarified);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_pivot_by_size)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Pivot_BySize);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_pivot_by_middle)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Pivot_ByMiddle);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_pivot_by_split_size)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Pivot_BySplitSize);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_strip)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Strip);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_strip_inverted)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Strip_Inverted);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_spiral)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Spiral);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_approximation)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Approximation);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_moore_greedy)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_Greedy);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_moore_minmax)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_MinMax);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_moore_minvariance)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Moore_MinVariance);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_hilbert_greedy)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_Greedy);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_hilbert_minmax)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_MinMax);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}


TEST_F(treemap_layouting_test, test_algorithm_hilbert_minvariance)
{
    layouter.setAlgorithm(RectangularLayoutAlgorithm::Hilbert_MinVariance);

    layouter.process(
        tree2.get(),
        *weight_values,
        layout
    );

    ASSERT_LAYOUT_PROPERTIES(tree2.get(), layout, *weight_values);
}
