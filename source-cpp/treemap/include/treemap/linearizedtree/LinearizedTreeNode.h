
#pragma once


#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include <cstdint>

#include <glm/vec4.hpp>

#include <treemap/treemap_api.h>


#ifndef LAYOUTER_INLINE
#define LAYOUTER_INLINE
#endif


class LinearizedTree;


class TREEMAP_API LinearizedTreeNode
{
public:
    struct NodeRange
    {
        const LinearizedTreeNode* first = nullptr;
        const LinearizedTreeNode* last = nullptr; // exclusive

        LAYOUTER_INLINE NodeRange(const LinearizedTreeNode* first, const LinearizedTreeNode* last)
        : first(first)
        , last(last)
        {
            assert(first != nullptr);
            assert(last != nullptr);
            assert(first <= last);
        }

        LAYOUTER_INLINE bool empty() const
        {
            assert(first <= last);

            return first == last;
        }

        LAYOUTER_INLINE std::size_t size() const
        {
            assert(first <= last);

            return last - first;
        }
    };

public:
    inline LinearizedTreeNode();
    inline LinearizedTreeNode(std::uint32_t index, std::uint32_t depth, std::uint32_t parent);
    inline LinearizedTreeNode(const LinearizedTreeNode & node);
    inline LinearizedTreeNode(LinearizedTreeNode && node);

    inline ~LinearizedTreeNode();

    inline LinearizedTreeNode & operator=(const LinearizedTreeNode & node);
    inline LinearizedTreeNode & operator=(LinearizedTreeNode && node);

    inline bool isLeaf() const;
    inline bool isRoot() const;

    inline std::uint32_t index() const;
    inline void setIndex(std::uint32_t index);

    inline std::uint32_t parent() const;
    inline void setParent(std::uint32_t index);

    inline std::uint32_t depth() const;
    inline void setDepth(std::uint16_t depth);

    inline std::uint32_t height() const;
    inline void setHeight(std::uint16_t height);

    inline std::uint32_t depthAndHeight() const;
    inline void setDepthAndHeight(std::uint32_t depthAndHeight);

    inline std::uint32_t firstChild() const;
    inline void setFirstChild(std::uint32_t nodeIndex);

    inline std::uint32_t numberOfChildren() const;
    inline std::uint32_t & numberOfChildren();
    inline void setNumberOfChildren(std::uint32_t count);

    inline operator glm::ivec4() const;

protected:
    std::uint32_t m_index;
    std::uint32_t m_depthAndHeight;
    std::uint32_t m_firstChild;
    std::uint32_t m_numberOfChildren;
    std::uint32_t m_parent;
};


#include <treemap/linearizedtree/LinearizedTreeNode.inl>
