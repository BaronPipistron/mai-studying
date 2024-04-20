#pragma once

#include <initializer_list>
#include <iostream>

#include "../allocator/Allocator.h"
#include "../../src/allocator/Allocator.cpp"
#include "../iterators/QueueIterator.h"
#include "../../src/iterators/QueueIterator.cpp"

using namespace iterators;

namespace containers {

template <class _Tp, class _Alloc = allocator::Allocator<_Tp>>
class Queue final {
  public:
    using value_type = _Tp;
    using allocator_type = _Alloc;
    using size_type = std::size_t;
    using reference = _Tp&;
    using const_reference = const _Tp&;
    using iterator = QueueIterator<_Tp>;
    using const_iterator = const QueueIterator<_Tp>;

  private:
    Node<_Tp>* _head;
    Node<_Tp>* _tail;
    typename _Alloc::template rebind<Node<value_type>>::other _alloc;
    size_type _size;

  public:
    Queue();
    Queue(const std::initializer_list<value_type>&);
    Queue(const Queue&);
    Queue(Queue&&) noexcept;

    Queue& operator=(const Queue&);
    Queue& operator=(Queue&&) noexcept;

    virtual ~Queue() noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    reference front();
    reference back();
    const_reference front() const;
    const_reference back() const;

    bool empty() const noexcept;
    size_type size() const noexcept;
    void clear();

    void push(const_reference);
    void pop();

    template <class _InputIt>
    void push_range(_InputIt, _InputIt);

    template <class... Args>
    void emplace(Args&&...);
};

}; // namespace containers