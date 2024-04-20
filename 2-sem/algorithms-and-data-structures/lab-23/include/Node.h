#ifndef NODEHPP
#define NODEHPP

#include <iostream>

template <typename T>
class BinaryTree;

template <typename T>
class Node{
    friend class BinaryTree<T>;
private:
    T data;
    Node<T>* left;
    Node<T>* right;

public:
    Node();
    Node(const T& value);
    Node(const T& value, Node<T>* left, Node<T>* right);
};

#include "../src/Node.cpp"

#endif