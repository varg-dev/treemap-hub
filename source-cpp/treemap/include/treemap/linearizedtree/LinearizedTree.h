
#pragma once


#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <iosfwd>

#include <treemap/linearizedtree/LinearizedTreeNode.h>
#include <treemap/buffers/LinearizedBuffer.h>

#include <treemap/treemap_api.h>


enum class IterationOrder : unsigned int
{
    ALL_NODES_TOP_DOWN,
    ALL_NODES_BOTTOM_UP,
    LEAVES,
    PARENTS_TOP_DOWN,
    PARENTS_BOTTOM_UP
};


class TREEMAP_API LinearizedTree
{
public:
    static const std::uint32_t invalidIndex;

    LinearizedTree();
	virtual ~LinearizedTree();

    std::uint32_t size() const;

    std::uint32_t numberOfParents() const;

    std::uint32_t numberOfLeaves() const;
    void setNumberOfLeaves(std::uint32_t count);

    std::uint32_t depth() const;

    LinearizedTreeNode * at(std::uint32_t index);
    const LinearizedTreeNode * at(std::uint32_t index) const;
    LinearizedTreeNode * operator[](std::uint32_t index);
    const LinearizedTreeNode * operator[](std::uint32_t index) const;

    LinearizedTreeNode::NodeRange nodesAsRange() const;
    LinearizedTreeNode::NodeRange childrenAsRange(const LinearizedTreeNode * parent) const;

    const LinearizedTreeNode* root() const;
    LinearizedTreeNode* root();

    template <typename Callback>
    void parentsDo(Callback callback) const;
    template <typename Callback>
    void reverseParentsDo(Callback callback) const;
    template <typename Callback>
    void leavesDo(Callback callback) const;
    template <typename Callback>
    void nodesDo(Callback callback) const;
    template <typename Callback>
    void reverseNodesDo(Callback callback) const;

    template <typename Callback>
    void nodesDepthFirstDo(Callback callback) const;
    template <typename Callback>
    void nodesDepthFirstDo(const LinearizedTreeNode * node, Callback callback) const;
    template <typename Callback>
    void childrenDo(const LinearizedTreeNode * node, Callback callback) const;

    bool hasAttribute(const std::string & name) const;
    void addAttribute(const std::string & name);
    LinearizedBuffer<float> & attributeValues(const std::string & name);
    const LinearizedBuffer<float> & attributeValues(const std::string & name) const;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices();
    const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices() const;

    const LinearizedBuffer<LinearizedTreeNode> & nodes() const;
    LinearizedBuffer<LinearizedTreeNode> & nodes();
    void setNodes(const LinearizedBuffer<LinearizedTreeNode> & nodes);
    void setNodes(LinearizedBuffer<LinearizedTreeNode> && nodes);

    void outputStatistics(std::ostream & stream) const;

    template <typename Callback, IterationOrder Order>
    void slicesDo(Callback && callback) const;

    template <typename Callback>
    void slicesDo(Callback && callback, IterationOrder order) const;

protected:
    std::uint32_t m_numberOfLeaves;

    LinearizedBuffer<LinearizedTreeNode> m_nodes;
    std::map<std::string, LinearizedBuffer<float>> m_attributes;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> m_slices;

    template <typename Callback>
    void sliceDo(std::uint32_t index, Callback callback) const;
};


#include <treemap/linearizedtree/LinearizedTree.inl>
