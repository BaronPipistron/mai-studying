#ifndef EXPRESSIONTREE_HPP
#define EXPRESSIONTREE_HPP

#include "Node.h"
#include "Stack.h"

#include <iostream>
#include <string>

class ExpressionTree{
private:
    Node* root;

public:
    ExpressionTree();
    ExpressionTree(const std::string& expression);
    ~ExpressionTree();

    Node* getRoot() const;

    void deleteTree(Node* node);
    Node* createTree(const std::string& postfix);
    std::string doPostfix(const std::string& expression);
    void replace(char var, const std::string& expression);
    Node* replace(Node* root, char var, Node* expression_tree);

    void printPostfix(Node* root) const;
    void printInfix(Node* root) const;
    void printTree(Node* root, const size_t height = 0) const;
};

#include "../src/ExpressionTree.cpp"

#endif