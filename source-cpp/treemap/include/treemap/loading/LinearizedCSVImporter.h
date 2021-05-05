
#pragma once


#include <string>
#include <vector>

#include <treemap/treemap_api.h>


class LinearizedTree;
class LinearizedTreeNode;


class TREEMAP_API LinearizedCSVImporter
{
public:
    LinearizedCSVImporter();
    LinearizedCSVImporter(bool enableVerboseOutput);

    LinearizedTree * load(const std::string& filename, const std::string & weightAttribute, std::vector<std::string>* paths = nullptr);

protected:
    bool m_enableVerboseOutput;
};
