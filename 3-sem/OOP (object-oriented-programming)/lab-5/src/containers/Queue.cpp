#include "../../include/containers/Queue.h"

using namespace containers;

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>::Queue():
    _head(nullptr), _tail(nullptr), _size(0)
{
}

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>::Queue(const std::initializer_list<_Tp>& list) :
    _head(nullptr), _tail(nullptr), _size(0)
{
    for (const auto& element: list) {
        push(element);
    }
}

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>::Queue(const Queue& other) :
    _head(nullptr), _tail(nullptr), _size(0)
{
    Node<_Tp>* cur = other._head;
    while (cur != nullptr) {
        this->push(cur->_data);
    }
}

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>::Queue(Queue<_Tp, _Alloc>&& other) noexcept
{
    this->_head = std::move(other._head);
    this->_tail = std::move(other._tail);
    this->_size = std::move(other._size);
}

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>& Queue<_Tp, _Alloc>::operator=(const Queue<_Tp, _Alloc>& other)
{
    if (this == &other) return *this;
    
    this->clear();
    
    Node<_Tp>* cur = other._head;
    while (cur != nullptr) {
        this->push(cur->_data);
    }

    return *this;
}

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>& Queue<_Tp, _Alloc>::operator=(Queue<_Tp, _Alloc>&& other) noexcept
{
    if (this == &other) return *this;

    this->clear();

    this->_head = std::move(other._head);
    this->_tail = std::move(other._tail);
    this->_size = std::move(other._size);

    return *this;
}

template <class _Tp, class _Alloc>
Queue<_Tp, _Alloc>::~Queue() noexcept
{
    clear();
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::iterator Queue<_Tp, _Alloc>::begin() 
{
    return QueueIterator(_head);
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::iterator Queue<_Tp, _Alloc>::end() 
{
    return QueueIterator(_tail);
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::const_iterator Queue<_Tp, _Alloc>::begin() const
{
    return QueueIterator(_head);
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::const_iterator Queue<_Tp, _Alloc>::end() const
{
    return QueueIterator(_tail);
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::reference Queue<_Tp, _Alloc>::front()
{
    return _head->_data;
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::reference Queue<_Tp, _Alloc>::back()
{
    return _tail->_data;
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::const_reference Queue<_Tp, _Alloc>::front() const
{
    return _head->_data;
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::const_reference Queue<_Tp, _Alloc>::back() const
{
    return _tail->_data;
}

template <class _Tp, class _Alloc>
bool Queue<_Tp, _Alloc>::empty() const noexcept
{
    return _size == 0;
}

template <class _Tp, class _Alloc>
typename Queue<_Tp, _Alloc>::size_type Queue<_Tp, _Alloc>::size() const noexcept
{
    return _size;
}

template <class _Tp, class _Alloc>
void Queue<_Tp, _Alloc>::clear() 
{
    while (!empty()) {
        pop();
    }
    _size = 0;
}

template <class _Tp, class _Alloc>
void Queue<_Tp, _Alloc>::push(const_reference __val)
{
    Node<_Tp>* new_Node = _alloc.allocate(1);
    _alloc.construct(new_Node, __val);
    new_Node->_next = nullptr;

    if (!_head) {
        _head = _tail = new_Node;
    } else {
        _tail->_next = new_Node;
        _tail = new_Node;
    }
    ++_size;
}

template <class _Tp, class _Alloc>
void Queue<_Tp, _Alloc>::pop()
{
    if (_size == 0) return;

    Node<_Tp>* old_Node = _head;
    _head = _head->_next;
    _alloc.destroy(old_Node);
    _alloc.deallocate(old_Node, 1);
    
    if (!_head) {
        _tail = nullptr;
    }
    --_size;
}

template <class _Tp, class _Alloc>
template <class _InputIt>
void Queue<_Tp, _Alloc>::push_range(_InputIt __begin, _InputIt __end)
{
    for (auto it = __begin; it != __end; ++it) {
        push(*it);
    }
}

template <class _Tp, class _Alloc>
template <class... Args>
void Queue<_Tp, _Alloc>::emplace(Args&&...__args)
{
    push(value_type(std::forward<Args>(__args)...));
}