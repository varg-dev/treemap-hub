
#pragma once


#include <treemap/linearizedtree/LinearizedTree.h>

#include <cassert>


namespace
{


template <IterationOrder Order>
struct tree_iteration
{
};

template <>
struct tree_iteration<IterationOrder::ALL_NODES_TOP_DOWN>
{
    template <typename Callback>
    void operator()(const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices, Callback && callback) const
    {
        for (auto i = 0; i < slices.size(); ++i)
        {
            callback(slices[i].first, slices[i].second);
        }
    }
};

template <>
struct tree_iteration<IterationOrder::ALL_NODES_BOTTOM_UP>
{
    template <typename Callback>
    void operator()(const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices, Callback && callback) const
    {
        for (auto i = slices.size(); i > 0; --i)
        {
            callback(slices[i-1].first, slices[i-1].second);
        }
    }
};

template <>
struct tree_iteration<IterationOrder::LEAVES>
{
    template <typename Callback>
    void operator()(const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices, Callback && callback) const
    {
        for (auto i = 0; i < slices.size(); ++i)
        {
            callback(slices[i].first, slices[i].second);
        }
    }
};

template <>
struct tree_iteration<IterationOrder::PARENTS_TOP_DOWN>
{
    template <typename Callback>
    void operator()(const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices, Callback && callback) const
    {
        for (auto i = 0; i < slices.size() - 2; ++i)
        {
            callback(slices[i].first, slices[i].second);
        }
    }
};

template <>
struct tree_iteration<IterationOrder::PARENTS_BOTTOM_UP>
{
    template <typename Callback>
    void operator()(const std::vector<std::pair<std::uint32_t, std::uint32_t>> & slices, Callback && callback) const
    {
        for (auto i = slices.size()-1; i > 0; --i)
        {
            callback(slices[i-1].first, slices[i-1].second);
        }
    }
};


} // namesspace


template <typename Callback>
void LinearizedTree::parentsDo(Callback callback) const
{
    if (m_slices.size() < 2)
    {
        return;
    }

    for (size_t i = 0; i < m_slices.size() - 1; ++i)
    {
        sliceDo(i, [& callback](const LinearizedTreeNode * node) {
            if (node->isLeaf())
            {
                return;
            }

            callback(node);
        });
    }
}

template <typename Callback>
void LinearizedTree::reverseParentsDo(Callback callback) const
{
    if (m_slices.size() < 2)
    {
        return;
    }

    for (auto i = static_cast<int>(m_slices.size() - 2); i >= 0 ; --i)
    {
        sliceDo(i, [& callback](const LinearizedTreeNode * node) {
            if (node->isLeaf())
            {
                return;
            }

            callback(node);
        });
    }
}

template <typename Callback>
void LinearizedTree::leavesDo(Callback callback) const
{
    for (size_t i = 0; i < m_slices.size(); ++i)
    {
        sliceDo(i, [& callback](const LinearizedTreeNode * node) {
            if (!node->isLeaf())
            {
                return;
            }

            callback(node);
        });
    }
}

template <typename Callback>
void LinearizedTree::nodesDo(Callback callback) const
{
    for (size_t i = 0; i < size(); ++i)
    {
        callback(at(i));
    }
}

template <typename Callback>
void LinearizedTree::nodesDepthFirstDo(Callback callback) const
{
    nodesDepthFirstDo(root(), callback);
}

template <typename Callback>
void LinearizedTree::nodesDepthFirstDo(const LinearizedTreeNode * node, Callback callback) const
{
    callback(node);

    if (node->isLeaf())
    {
        return;
    }

    childrenDo(node, [this, &callback](const LinearizedTreeNode * node) {
        nodesDepthFirstDo(node, callback);
    });
}

template <typename Callback>
void LinearizedTree::childrenDo(const LinearizedTreeNode * node, Callback callback) const
{
    if (node->isLeaf())
    {
        return;
    }

    auto current = at(node->firstChild());
    for (auto i = std::uint32_t(0); i < node->numberOfChildren(); ++i, ++current)
    {
        callback(current);
    }
}

template <typename Callback>
void LinearizedTree::reverseNodesDo(Callback callback) const
{
    if (size() == 0)
    {
        return;
    }

    for (size_t i = size()-1; i > 0; --i)
    {
        callback(at(i));
    }

    callback(at(0));
}

template <typename Callback>
void LinearizedTree::sliceDo(std::uint32_t index, Callback callback) const
{
    if (index >= m_slices.size())
    {
        return;
    }

    const auto & pair = m_slices[index];

    for (auto i = pair.first; i <= pair.second; ++i)
    {
        callback(at(i));
    }
}

/*
template <typename Callback, IterationOrder Order>
void LinearizedTree::slicesDo(Callback callback) const;
*/

template <typename Callback, IterationOrder Order>
void LinearizedTree::slicesDo(Callback && callback) const
{
    tree_iteration<Order>()(m_slices, std::move(callback));
}

template <typename Callback>
void LinearizedTree::slicesDo(Callback && callback, IterationOrder order) const
{
    switch (order)
    {
    case IterationOrder::ALL_NODES_TOP_DOWN:
        tree_iteration<IterationOrder::ALL_NODES_TOP_DOWN>()(m_slices, std::move(callback));
        break;
    case IterationOrder::ALL_NODES_BOTTOM_UP:
        tree_iteration<IterationOrder::ALL_NODES_BOTTOM_UP>()(m_slices, std::move(callback));
        break;
    case IterationOrder::LEAVES:
        tree_iteration<IterationOrder::LEAVES>()(m_slices, std::move(callback));
        break;
    case IterationOrder::PARENTS_TOP_DOWN:
        tree_iteration<IterationOrder::PARENTS_TOP_DOWN>()(m_slices, std::move(callback));
        break;
    case IterationOrder::PARENTS_BOTTOM_UP:
        tree_iteration<IterationOrder::PARENTS_BOTTOM_UP>()(m_slices, std::move(callback));
        break;
    default:
        break;
    }
}
