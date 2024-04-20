#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>

template <typename T>
class Node{
private:
    T val;
    Node<T>* next;

public:
    Node() = default;
    Node(const T& val);
    Node(const T& val, Node<T>* next);
    
    T& get_val() const;
    Node<T>* get_next() const;

    void set_val(const T& val);
    void set_next(Node<T>* next);
};

#include "../src/Node.cpp"

#endif