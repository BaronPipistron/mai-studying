#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>

template <typename T>
class Node{
    friend class Iterator<T>;
    friend class Deque<T>;

private:
    T data;
    Node<T>* next;
    Node<T>* previous;

public:
    Node();
    Node(const T& value);
    Node(const T& value, Node<T>* previous, Node<T>* current);
};

#include "../src/Node.cpp"

#endif