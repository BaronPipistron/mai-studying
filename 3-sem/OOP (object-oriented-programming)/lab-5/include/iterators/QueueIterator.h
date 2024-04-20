#pragma once

#include <cstddef>
#include <iterator>

template <class _Tp>
struct Node {
    _Tp _data;
    Node* _next;
    Node(): _data(_Tp()), _next(nullptr) {}
    Node(const _Tp& value): _data(value), _next(nullptr) {}
    Node(const Node& other): _data(other._data), _next(other._next) {}
};

namespace iterators {

template <class _Tp>
class QueueIterator final {
  public:
    using value_type = _Tp;
    using pointer = _Tp*;
    using const_pointer = const _Tp*;
    using reference = _Tp&;
    using const_reference = const _Tp&;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    template <class _Tp1, class _Alloc1>
    friend class Queue;

  private:
    Node<value_type>* _node;

  public:
    QueueIterator() = delete;
    QueueIterator(Node<value_type>*);
    virtual ~QueueIterator() = default;

    QueueIterator& operator++() noexcept;
    QueueIterator operator++(int) noexcept;

    reference operator*();
    pointer operator->();
    const_reference operator*() const;
    const_pointer operator->() const;

    bool operator==(const QueueIterator&) const noexcept;
    bool operator!=(const QueueIterator&) const noexcept;
};

}; // namespace iterators