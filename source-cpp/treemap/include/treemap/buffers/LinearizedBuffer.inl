
#pragma once


#include <treemap/buffers/LinearizedBuffer.h>


template <typename T>
LinearizedBuffer<T>::LinearizedBuffer()
{
}

template <typename T>
LinearizedBuffer<T>::LinearizedBuffer(const std::vector<T>& data)
: m_data(data)
{
}

template <typename T>
LinearizedBuffer<T>::LinearizedBuffer(std::vector<T>&& data) 
: m_data(std::move(data))
{
}

template <typename T>
auto LinearizedBuffer<T>::begin() -> iterator
{
    return m_data.begin();
}

template <typename T>
auto LinearizedBuffer<T>::begin() const -> const_iterator
{
    return m_data.begin();
}

template <typename T>
auto LinearizedBuffer<T>::end() -> iterator
{
    return m_data.end();
}

template <typename T>
auto LinearizedBuffer<T>::end() const -> const_iterator
{
    return m_data.end();
}

template <typename T>
auto LinearizedBuffer<T>::size() const -> size_type
{
    return static_cast<size_type>(m_data.size());
}

template <typename T>
void LinearizedBuffer<T>::resize(const size_type size)
{
    m_data.resize(static_cast<size_type>(size));
}

template <typename T>
bool LinearizedBuffer<T>::empty() const
{
    return m_data.empty();
}

template <typename T>
void LinearizedBuffer<T>::clear()
{
    m_data.clear();
}

template <typename T>
auto LinearizedBuffer<T>::operator[](const LinearizedTreeNode * node) -> reference
{
    return m_data[indexOf(node)];
}

template <typename T>
auto LinearizedBuffer<T>::operator[](const LinearizedTreeNode * node) const -> const_reference
{
    return m_data[indexOf(node)];
}

template <typename T>
auto LinearizedBuffer<T>::at(const LinearizedTreeNode * node) -> reference
{
    return m_data[indexOf(node)];
}

template <typename T>
auto LinearizedBuffer<T>::at(const LinearizedTreeNode * node) const -> const_reference
{
    return m_data[indexOf(node)];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto LinearizedBuffer<T>::operator[](IndexType index) -> reference
{
    return m_data[index];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto LinearizedBuffer<T>::operator[](IndexType index) const -> const_reference
{
    return m_data[index];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto LinearizedBuffer<T>::at(IndexType index) -> reference
{
    return m_data[index];
}

template <typename T>
template <typename IndexType, typename std::enable_if<std::is_integral<IndexType>::value>::type *>
auto LinearizedBuffer<T>::at(IndexType index) const -> const_reference
{
    return m_data[index];
}

template <typename T>
auto LinearizedBuffer<T>::front() -> reference
{
    return m_data.front();
}

template <typename T>
auto LinearizedBuffer<T>::front() const -> const_reference
{
    return m_data.front();
}

template <typename T>
auto LinearizedBuffer<T>::back() -> reference
{
    return m_data.back();
}

template <typename T>
auto LinearizedBuffer<T>::back() const -> const_reference
{
    return m_data.back();
}

template <typename T>
T *  LinearizedBuffer<T>::data()
{
    return m_data.data();
}

template <typename T>
const T * LinearizedBuffer<T>::data() const
{
    return m_data.data();
}

template <typename T>
std::vector<T> & LinearizedBuffer<T>::vector()
{
    return m_data;
}

template <typename T>
const std::vector<T> & LinearizedBuffer<T>::vector() const
{
    return m_data;
}

template <typename T>
void LinearizedBuffer<T>::setVector(const std::vector<T> & vector)
{
    m_data = vector;
}

template <typename T>
void LinearizedBuffer<T>::setVector(std::vector<T> && vector)
{
    m_data = std::move(vector);
}
