
#pragma once


#include <cstdint>

#include <treemap/treemap_api.h>


class LinearizedTreeNode;


class TREEMAP_API AbstractLinearizedBuffer
{
public:
    AbstractLinearizedBuffer();

    std::uint32_t indexOf(const LinearizedTreeNode *node) const;
};
