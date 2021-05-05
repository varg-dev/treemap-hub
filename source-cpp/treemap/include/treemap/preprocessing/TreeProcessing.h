
#pragma once


#include <string>
#include <vector>

#include <treemap/linearizedtree/LinearizedTree.h>

#include <treemap/treemap_api.h>


class TREEMAP_API TreeProcessing
{
public:
    TreeProcessing();

    LinearizedTree * removeZeroWeightNodes(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
    LinearizedTree * removeZeroWeightNodesAndDeleteSource(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
    LinearizedTree * flatten(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
    LinearizedTree * flattenAndDeleteSource(LinearizedTree * linearizedTree, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);
};
