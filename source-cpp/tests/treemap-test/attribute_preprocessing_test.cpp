
#include <gmock/gmock.h>

#include <numeric>

#include <treemap/loading/LinearizedCSVImporter.h>

#include <treemap/linearizedtree/LinearizedTree.h>
#include <treemap/preprocessing/AttributeSumAggregation.h>


class preprocess_attributes_test : public testing::Test
{
public:
    preprocess_attributes_test()
    {
        LinearizedCSVImporter linearizedImporter;
        const std::string filename1 = "../../datasets/glbinding-former.csv";

        tree1.reset(linearizedImporter.load(filename1, weightAttribute, nullptr));
    }

public:
    const std::string weightAttribute = "RLoc";

    std::unique_ptr<LinearizedTree> tree1 = nullptr;

    AttributeSumAggregation aggregation;
};


TEST_F(preprocess_attributes_test, aggregate_weights)
{
    auto weight_values = tree1->attributeValues(weightAttribute);

    ASSERT_EQ(0.0f, weight_values[0]);

    const auto sum = std::accumulate(weight_values.begin(), weight_values.end(), 0.0f);

    aggregation.process(tree1.get(), weight_values);

    ASSERT_LT(0.0f, weight_values[0]);
    ASSERT_NEAR(sum, weight_values[0], 0.000000001f);
}
