#include "../../include/iterators/QueueIterator.h"

using namespace iterators;

template <class _Tp>
QueueIterator<_Tp>::QueueIterator(Node<value_type>* node) :
    _node(node)
{
}

template <class _Tp>
QueueIterator<_Tp>& QueueIterator<_Tp>::operator++() noexcept
{
    _node = _node->_next;
    return *this;
}

template <class _Tp>
QueueIterator<_Tp> QueueIterator<_Tp>::operator++(int) noexcept
{
    QueueIterator<_Tp> tmp = *this;
    ++(*this);
    return tmp;
}

template <class _Tp>
typename QueueIterator<_Tp>::reference QueueIterator<_Tp>::operator*()
{
    return _node->_data;
}

template <class _Tp>
typename QueueIterator<_Tp>::pointer QueueIterator<_Tp>::operator->()
{
    return &(_node->_data);
}

template <class _Tp>
typename QueueIterator<_Tp>::const_reference QueueIterator<_Tp>::operator*() const
{
    return _node->_data;
}

template <class _Tp>
typename QueueIterator<_Tp>::const_pointer QueueIterator<_Tp>::operator->() const
{
    return &(_node->_data);
}

template <class _Tp>
bool QueueIterator<_Tp>::operator==(const QueueIterator<_Tp>& other) const noexcept
{
    return &this->_node == &other._node;
}

template <class _Tp>
bool QueueIterator<_Tp>::operator!=(const QueueIterator<_Tp>& other) const noexcept
{
    return &this->_node != &other._node;
}