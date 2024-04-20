#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "Node.h"

template <typename T>
class List;

template <typename T>
class Iterator{
    friend class List<T>;
private:
    Node<T>* node;

public:
    Iterator() = default;
    Iterator(Node<T>* node);
    Iterator(const List<T>& lst);
    ~Iterator() = default;

    Iterator<T>& operator++();
    Iterator<T>& operator+(size_t value);

    T& operator*();
    const T& operator*() const;

    bool operator==(const Iterator<T>& other) const;
    bool operator!=(const Iterator<T>& other) const;
};

#include "../src/Iterator.cpp"

#endif