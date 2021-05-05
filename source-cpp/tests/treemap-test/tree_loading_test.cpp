
#include <gmock/gmock.h>

#include <treemap/loading/LinearizedCSVImporter.h>

#include <treemap/linearizedtree/LinearizedTree.h>


class tree_loading_test : public testing::Test
{
public:
    tree_loading_test()
    {
    }

public:
    LinearizedCSVImporter linearizedImporter;
    const std::string filename1 = "../../datasets/csv/glbinding-former.csv";
    const std::string filename2 = "../../datasets/csv/glbinding-latest.csv";
    const std::string weightAttribute = "RLoc";
    std::vector<std::string> paths;
};


TEST_F(tree_loading_test, load_tree)
{
    const auto tree1 = std::unique_ptr<LinearizedTree>(linearizedImporter.load(filename1, "", nullptr));

    ASSERT_NE(nullptr, tree1);
    ASSERT_LT(0, tree1->numberOfParents());
    ASSERT_LT(0, tree1->numberOfLeaves());
}


TEST_F(tree_loading_test, load_tree_with_weights)
{
    const auto tree1 = std::unique_ptr<LinearizedTree>(linearizedImporter.load(filename1, weightAttribute, nullptr));

    ASSERT_NE(nullptr, tree1);
    ASSERT_LT(0, tree1->numberOfParents());
    ASSERT_LT(0, tree1->numberOfLeaves());

    ASSERT_TRUE(tree1->hasAttribute(weightAttribute));

    const auto weight_values = tree1->attributeValues(weightAttribute);

    ASSERT_EQ(tree1->numberOfLeaves() + tree1->numberOfParents(), weight_values.size());
}


TEST_F(tree_loading_test, load__multiple_trees)
{
    const auto tree1 = std::unique_ptr<LinearizedTree>(linearizedImporter.load(filename1, "", nullptr));

    ASSERT_NE(nullptr, tree1);
    ASSERT_LT(0, tree1->numberOfParents());
    ASSERT_LT(0, tree1->numberOfLeaves());

    const auto tree2 = std::unique_ptr<LinearizedTree>(linearizedImporter.load(filename2, "", nullptr));

    ASSERT_NE(nullptr, tree2);
    ASSERT_LT(0, tree2->numberOfParents());
    ASSERT_LT(0, tree2->numberOfLeaves());
}


TEST_F(tree_loading_test, load_multiple_trees_with_persistent_paths)
{
    const auto tree1 = std::unique_ptr<LinearizedTree>(linearizedImporter.load(filename1, "", &paths));

    ASSERT_NE(nullptr, tree1);
    ASSERT_LT(0, tree1->numberOfParents());
    ASSERT_LT(0, tree1->numberOfLeaves());

    ASSERT_LT(0, paths.size());
    const auto prev_size = paths.size();

    const auto tree2 = std::unique_ptr<LinearizedTree>(linearizedImporter.load(filename2, "", &paths));

    ASSERT_NE(nullptr, tree2);
    ASSERT_LT(0, tree2->numberOfParents());
    ASSERT_LT(0, tree2->numberOfLeaves());

    ASSERT_LT(prev_size, paths.size());
}
