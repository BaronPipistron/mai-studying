#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "Node.h"

#include <iostream>

template <typename T>
class Deque;

template <typename T>
class Iterator{
    friend class Deque<T>;
private:
    Node<T>* node;

public:
    Iterator() = default;
    Iterator(Node<T>* node);
    ~Iterator() = default;

    Node<T>* get_node();

    T& operator*();
    const T& operator*() const;

    Iterator<T> operator++();
    Iterator<T> operator--();
    Iterator<T> operator+(int64_t value);
    Iterator<T> operator-(int64_t value);

    bool operator==(const Iterator<T>& other);
    bool operator!=(const Iterator<T>& other);
};

#include "../src/Iterator.cpp"

#endif