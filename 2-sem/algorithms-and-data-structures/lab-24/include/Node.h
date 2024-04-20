#ifndef NODE_HPP
#define NODE_HPP

class ExpressionTree;

class Node{
    friend class ExpressionTree;

private:
    char data;
    Node* left;
    Node* right;

public:
    Node();
    Node(const char data);
    Node(const char data, Node* left, Node* right);
};

#include "../src/Node.cpp"

#endif