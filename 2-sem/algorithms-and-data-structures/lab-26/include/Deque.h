#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "Node.h"
#include "Iterator.h"

#include <iostream>

template <typename t>
class Iterator;

template <typename T>
class Deque{
    friend class Iterator<T>;

private:
    Node<T>* head;
    Node<T>* tail;
    size_t sz;

public:
    Deque();
    ~Deque();

    Iterator<T> begin() const;
    Iterator<T> end() const;

    void push_front(const T& value);
    void push_back(const T& value);

    void pop_front();
    void pop_back();

    size_t size() const;
    bool empty() const;
    void clear();

    void sort();

    void print() const;
};

#include "../src/Deque.cpp"

#endif