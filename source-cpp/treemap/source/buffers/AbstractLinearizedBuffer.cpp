
#include <treemap/buffers/AbstractLinearizedBuffer.h>

#include <treemap/linearizedtree/LinearizedTreeNode.h>


AbstractLinearizedBuffer::AbstractLinearizedBuffer()
{
}

std::uint32_t AbstractLinearizedBuffer::indexOf(const LinearizedTreeNode *node) const
{
    return node->index();
}
