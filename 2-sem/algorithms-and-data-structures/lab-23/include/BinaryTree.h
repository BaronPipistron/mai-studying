#ifndef BINARYTREEHPP
#define BINARYTREEHPP

#include "Node.h"
#include "../../course-project-7/include/MyVector.h"

template <typename T>
class BinaryTree{
private:
    Node<T>* root;
    Node<T>* findMin(Node<T>* root);

public:
    BinaryTree();
    BinaryTree(const T& value);
    ~BinaryTree();

    Node<T>* getRoot() const;
    void deleteTree(Node<T>* node);
    void insertNode(const T& value, Node<T>* root);
    Node<T>* removeNode(const T& value, Node<T>* root);

    void visualize(Node<T>* root, const int height = 0) const;
    void inOrderBypass(Node<T>* root);
    bool searchNode(const T& value, Node<T>* root);

    int getMaxWidth(Node<T>* root);
    int getWidth(Node<T>* root, int level, myVector<T>& levels);
    int getHeight(Node<T>* root);
};

#include "../src/BinaryTree.cpp"

#endif