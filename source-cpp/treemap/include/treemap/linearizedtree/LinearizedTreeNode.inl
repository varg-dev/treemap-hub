
#pragma once


#include <treemap/linearizedtree/LinearizedTreeNode.h>


LinearizedTreeNode::LinearizedTreeNode()
: m_index(static_cast<std::uint32_t>(-1))
, m_depthAndHeight(0)
, m_firstChild(static_cast<std::uint32_t>(-1))
, m_numberOfChildren(0)
, m_parent(static_cast<std::uint32_t>(-1))
{
}

LinearizedTreeNode::LinearizedTreeNode(std::uint32_t index, std::uint32_t depth, std::uint32_t parent)
: m_index(index)
, m_depthAndHeight(depth << 16u | 0)
, m_firstChild(static_cast<std::uint32_t>(-1))
, m_numberOfChildren(0)
, m_parent(parent)
{
}

LinearizedTreeNode::LinearizedTreeNode(const LinearizedTreeNode & node)
: m_index(node.index())
, m_depthAndHeight(node.depthAndHeight())
, m_firstChild(node.firstChild())
, m_numberOfChildren(node.numberOfChildren())
, m_parent(node.parent())
{
}

LinearizedTreeNode::LinearizedTreeNode(LinearizedTreeNode && node)
: m_index(node.index())
, m_depthAndHeight(node.depthAndHeight())
, m_firstChild(node.firstChild())
, m_numberOfChildren(node.numberOfChildren())
, m_parent(node.parent())
{
}

LinearizedTreeNode::~LinearizedTreeNode()
{
}

std::uint32_t LinearizedTreeNode::index() const
{
    return m_index;
}

void LinearizedTreeNode::setIndex(std::uint32_t index)
{
    m_index = index;
}

bool LinearizedTreeNode::isRoot() const
{
    return m_index == 0;
}

std::uint32_t LinearizedTreeNode::parent() const
{
    return m_parent;
}

void LinearizedTreeNode::setParent(std::uint32_t parent)
{
    m_parent = parent;
}

void LinearizedTreeNode::setDepth(std::uint16_t depth)
{
    setDepthAndHeight(depth << 16u | height());
}

void LinearizedTreeNode::setHeight(std::uint16_t height)
{
    setDepthAndHeight(depth() << 16u | height);
}

void LinearizedTreeNode::setDepthAndHeight(std::uint32_t depthAndHeight)
{
    m_depthAndHeight = depthAndHeight;
}

void LinearizedTreeNode::setFirstChild(std::uint32_t nodeIndex)
{
    m_firstChild = nodeIndex;
}

void LinearizedTreeNode::setNumberOfChildren(std::uint32_t count)
{
    m_numberOfChildren = count;
}

std::uint32_t LinearizedTreeNode::depth() const
{
    return m_depthAndHeight >> 16u;
}

std::uint32_t LinearizedTreeNode::height() const
{
    return m_depthAndHeight & 0xFFFF;
}

std::uint32_t LinearizedTreeNode::depthAndHeight() const
{
    return m_depthAndHeight;
}

LinearizedTreeNode & LinearizedTreeNode::operator=(const LinearizedTreeNode & node)
{
    m_index = node.index();
    m_depthAndHeight = node.depthAndHeight();
    m_firstChild = node.firstChild();
    m_numberOfChildren = node.numberOfChildren();
    m_parent = node.parent();

    return *this;
}

LinearizedTreeNode & LinearizedTreeNode::operator=(LinearizedTreeNode && node)
{
    m_index = node.index();
    m_depthAndHeight = node.depthAndHeight();
    m_firstChild = node.firstChild();
    m_numberOfChildren = node.numberOfChildren();
    m_parent = node.parent();

    return *this;
}

bool LinearizedTreeNode::isLeaf() const
{
    return m_firstChild == static_cast<std::uint32_t>(-1);
}

std::uint32_t LinearizedTreeNode::numberOfChildren() const
{
    return m_numberOfChildren;
}

std::uint32_t & LinearizedTreeNode::numberOfChildren()
{
    return m_numberOfChildren;
}

std::uint32_t LinearizedTreeNode::firstChild() const
{
    return m_firstChild;
}

LinearizedTreeNode::operator glm::ivec4() const
{
    return glm::ivec4(m_index, m_depthAndHeight, m_firstChild, m_numberOfChildren);
}
