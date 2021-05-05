
#include <treemap/linearizedtree/LinearizedTree.h>

#include <cassert>

#include <iostream>


const std::uint32_t LinearizedTree::invalidIndex = static_cast<std::uint32_t>(-1);

LinearizedTree::LinearizedTree()
: m_numberOfLeaves(0)
{
}

LinearizedTree::~LinearizedTree()
{
}

std::uint32_t LinearizedTree::size() const
{
    return m_nodes.size();
}

std::uint32_t LinearizedTree::numberOfParents() const
{
    return size() - numberOfLeaves();
}

void LinearizedTree::setNumberOfLeaves(std::uint32_t count)
{
    m_numberOfLeaves = count;
}

std::uint32_t LinearizedTree::numberOfLeaves() const
{
    return m_numberOfLeaves;
}

std::uint32_t LinearizedTree::depth() const
{
    return m_slices.size();
}

LinearizedTreeNode * LinearizedTree::at(std::uint32_t index)
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

const LinearizedTreeNode * LinearizedTree::at(std::uint32_t index) const
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

LinearizedTreeNode * LinearizedTree::operator[](std::uint32_t index)
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

const LinearizedTreeNode * LinearizedTree::operator[](std::uint32_t index) const
{
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    return &m_nodes.at(index);
}

LinearizedTreeNode::NodeRange LinearizedTree::nodesAsRange() const
{
    return { root(), root() + size() };
}

LinearizedTreeNode::NodeRange LinearizedTree::childrenAsRange(const LinearizedTreeNode * parent) const
{
    const auto firstChild = at(parent->firstChild());

    return { firstChild, firstChild+parent->numberOfChildren() };
}

const LinearizedTreeNode* LinearizedTree::root() const
{
    return &m_nodes.front();
}

LinearizedTreeNode* LinearizedTree::root()
{
    return &m_nodes.front();
}

bool LinearizedTree::hasAttribute(const std::string & name) const
{
    return m_attributes.find(name) != m_attributes.end();
}

void LinearizedTree::addAttribute(const std::string & name)
{
    m_attributes.emplace(name, LinearizedBuffer<float>(std::vector<float>()));
}

LinearizedBuffer<float> & LinearizedTree::attributeValues(const std::string & name)
{
    return m_attributes.at(name);
}

const LinearizedBuffer<float> & LinearizedTree::attributeValues(const std::string & name) const
{
    return m_attributes.at(name);
}

std::vector<std::pair<std::uint32_t, std::uint32_t>> & LinearizedTree::slices()
{
    return m_slices;
}

const std::vector<std::pair<std::uint32_t, std::uint32_t>> & LinearizedTree::slices() const
{
    return m_slices;
}

const LinearizedBuffer<LinearizedTreeNode> & LinearizedTree::nodes() const
{
    return m_nodes;
}

LinearizedBuffer<LinearizedTreeNode> & LinearizedTree::nodes()
{
    return m_nodes;
}

void LinearizedTree::setNodes(const LinearizedBuffer<LinearizedTreeNode> & nodes)
{
    m_nodes = nodes;
}

void LinearizedTree::setNodes(LinearizedBuffer<LinearizedTreeNode> && nodes)
{
    m_nodes = std::move(nodes);
}

void LinearizedTree::outputStatistics(std::ostream & stream) const
{
    stream << "Number of nodes: " << size() << '\n';
    stream << "Number of parents: " << numberOfParents() << '\n';
    stream << "Number of leaves: " << numberOfLeaves() << '\n';
    stream << "Maximum depth: " << depth() << '\n';
    stream.flush();
}
