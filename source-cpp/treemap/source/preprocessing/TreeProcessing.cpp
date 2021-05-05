
#include <treemap/preprocessing/TreeProcessing.h>

#include <numeric>

#include <treemap/linearizedtree/LinearizedTreeNode.h>


TreeProcessing::TreeProcessing()
{
}


LinearizedTree * TreeProcessing::removeZeroWeightNodesAndDeleteSource(LinearizedTree *linearizedTree, const std::string &weightAttribute, std::vector<std::string> *paths)
{
    const auto result = removeZeroWeightNodes(linearizedTree, weightAttribute, paths);

    delete linearizedTree;

    return result;
}


LinearizedTree * TreeProcessing::removeZeroWeightNodes(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* paths)
{
    if (linearizedTree == nullptr || linearizedTree->size() < 1)
    {
        delete linearizedTree;
        return nullptr;
    }

    auto prunedLinearizedTree = new LinearizedTree();
    prunedLinearizedTree->addAttribute(weightAttribute);

    const auto & weightValues = linearizedTree->attributeValues(weightAttribute);

    constexpr auto zeroWeightThreshold = 0.0f;

    std::vector<std::size_t> subtreeSize(linearizedTree->size());
    auto numberOfLeaves = std::size_t(0);
    linearizedTree->reverseNodesDo([& subtreeSize, &weightValues, &numberOfLeaves](const LinearizedTreeNode * node) {
        if (node->isLeaf())
        {
            numberOfLeaves += static_cast<std::size_t>(weightValues[node] > zeroWeightThreshold);
            subtreeSize[node->index()] = static_cast<std::size_t>(weightValues[node] > zeroWeightThreshold);
        }
        else
        {
            subtreeSize[node->index()] = std::accumulate(subtreeSize.begin() + node->firstChild(), subtreeSize.begin() + node->firstChild() + node->numberOfChildren(), std::size_t(0));

            if (subtreeSize[node->index()] > 0)
            {
                ++subtreeSize[node->index()];
            }
        }
    });

    if (subtreeSize.front() < 1)
    {
        delete linearizedTree;
        return nullptr;
    }

    std::vector<LinearizedTreeNode> nodes(subtreeSize.front());
    std::vector<LinearizedTreeNode *> previousNodes(nodes.size());
    auto & prunedWeightValues = prunedLinearizedTree->attributeValues(weightAttribute);
    prunedWeightValues.setVector(std::vector<float>(nodes.size()));
    previousNodes[0] = linearizedTree->at(0);
    prunedLinearizedTree->setNumberOfLeaves(numberOfLeaves);

    auto nextNodeId = std::size_t(1);
    auto & slices = prunedLinearizedTree->slices();
    auto startOfNextSlice = std::size_t(1);
    slices.emplace_back(0, 0);
    for (auto i = std::size_t(0); i < nodes.size(); ++i)
    {
        if (startOfNextSlice == i)
        {
            slices.emplace_back(startOfNextSlice, nextNodeId-1);
            startOfNextSlice = nextNodeId;
        }
        const auto previousNode = previousNodes[i];

        prunedWeightValues.at(i) = weightValues.at(previousNode);

        nodes[i].setIndex(i);
        nodes[i].setDepth(slices.size()-1);

        if (previousNode->isLeaf())
        {
            continue;
        }

        nodes[i].setFirstChild(nextNodeId);
        for (auto childI = previousNode->firstChild(); childI < previousNode->firstChild() + previousNode->numberOfChildren(); ++childI)
        {
            if (subtreeSize[childI] == 0)
            {
                continue;
            }

            previousNodes[nextNodeId] = linearizedTree->at(childI);
            nodes[nextNodeId].setParent(i);
            ++nextNodeId;
        }
        nodes[i].setNumberOfChildren(nextNodeId - nodes[i].firstChild());
    }
    slices.back().second = nextNodeId-1;
    prunedLinearizedTree->setNodes(LinearizedBuffer<LinearizedTreeNode>(std::move(nodes)));

    if (paths)
    {
        assert(paths->size() == linearizedTree->size());
        for (int i = 0; i < prunedLinearizedTree->size(); ++i) {
            assert(previousNodes[i]->index() >= i);
            paths->operator[](i) = paths->operator[](previousNodes[i]->index());
        }
        paths->resize(prunedLinearizedTree->size());
    }

    return prunedLinearizedTree;
}


LinearizedTree * TreeProcessing::flattenAndDeleteSource(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* paths)
{
    const auto result = flatten(linearizedTree, weightAttribute, paths);

    delete linearizedTree;

    return result;
}


// TODO: update paths
LinearizedTree * TreeProcessing::flatten(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* /*paths*/)
{
    auto flattedLinearizedTree = new LinearizedTree();
    flattedLinearizedTree->setNumberOfLeaves(linearizedTree->numberOfLeaves());
    flattedLinearizedTree->addAttribute(weightAttribute);

    const auto & weightValues = linearizedTree->attributeValues(weightAttribute);

    std::vector<LinearizedTreeNode> nodes;
    auto & slices = flattedLinearizedTree->slices();
    auto & flattedWeightValues = flattedLinearizedTree->attributeValues(weightAttribute);
    flattedWeightValues.setVector(std::vector<float>(linearizedTree->numberOfLeaves() + 1));

    slices.resize(2);
    slices.front() = { 0, 0 };
    slices.back() = { 1, linearizedTree->numberOfLeaves() };

    nodes.emplace_back(0, 0, -1);

    auto & root = nodes.front();

    linearizedTree->leavesDo([&nodes, &root, &weightValues, &flattedWeightValues](const LinearizedTreeNode * leaf) {
        nodes.emplace_back(nodes.size(), 1, 0);
        flattedWeightValues[nodes.back().index()] = weightValues[leaf];
    });
    nodes[0].setFirstChild(1);
    nodes[0].setNumberOfChildren(linearizedTree->numberOfLeaves());
    flattedWeightValues[0] = 0.0f;
    flattedLinearizedTree->setNodes(LinearizedBuffer<LinearizedTreeNode>(std::move(nodes)));

    return flattedLinearizedTree;
}
