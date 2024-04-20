#include "../include/BinaryTree.h"

template <typename T>
BinaryTree<T>::BinaryTree(): root(nullptr){}

template <typename T>
BinaryTree<T>::BinaryTree(const T& value){
    this->root = new Node<T>(value);
}

template <typename T>
BinaryTree<T>::~BinaryTree(){
    deleteTree(root);
}

template <typename T>
Node<T>* BinaryTree<T>::getRoot() const{
    return this->root;
}

template <typename T>
void BinaryTree<T>::deleteTree(Node<T>* node){
    if (node == nullptr) return;

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

template <typename T>
void BinaryTree<T>::insertNode(const T& value, Node<T>* root){
    if (value < root->data){
        if (root->left == nullptr){
            root->left = new Node<T>(value);
        } else{
            insertNode(value, root->left);
        }
    } else{
        if (root->right == nullptr){
            root->right = new Node<T>(value);
        } else{
            insertNode(value, root->right);
        }
    }
    return;
}

template <typename T>
Node<T>* BinaryTree<T>::removeNode(const T& value, Node<T>* root){
    if (root == nullptr) return root;

    if (value < root->data){
        root->left = removeNode(value, root->left);
    } else if (value > root->data){
        root->right = removeNode(value, root->right);
    } else{
        if (root->left == nullptr && root->right == nullptr){
            delete root;
            return nullptr;
        } else if(root->left == nullptr){
            Node<T>* tmp = root->right;
            delete root;
            return tmp;
        } else if (root->right == nullptr){
            Node<T>* tmp = root->left;
            delete root;
            return tmp;
        }
        Node<T>* tmp = findMin(root->right);
        root->data = tmp->data;
        root->right = removeNode(tmp->data, root->right);
    }
    return root;
}

template <typename T>
Node<T>* BinaryTree<T>::findMin(Node<T>* root){
    while (root->left != nullptr){
        root = root->left;
    }
    return root;
}

template <typename T>
void BinaryTree<T>::visualize(Node<T>* root, const int height) const{
    if (root != nullptr){
        visualize(root->right, height + 1);
        for (int i = 0; i < height; ++i){
            std::cout << "\t";
        }
        std::cout << root->data << "\n";
        visualize(root->left, height + 1);
    }
}

template <typename T>
void BinaryTree<T>::inOrderBypass(Node<T>* root){
    if (root == nullptr) return;

    inOrderBypass(root->left);
    std::cout << root->data << " ";
    inOrderBypass(root->right);
}

template <typename T>
bool BinaryTree<T>::searchNode(const T& value, Node<T>* root){
    if (root == nullptr) return false;

    if (value == root->data) return true;
    if (value < root->data){
        searchNode(value, root->left);
    } else{
        searchNode(value, root->right);
    }
}

template <typename T>
int BinaryTree<T>::getMaxWidth(Node<T>* root){
    int height = getHeight(root);
    int max_width = 0;
    myVector<T> levels;
    levels.resize(height);

    getWidth(root, 0, levels);
    for (size_t i = 0; i != height; ++i){
        if (levels[i] > max_width) max_width = levels[i];
    }

    return max_width;
}

template <typename T>
int BinaryTree<T>::getWidth(Node<T>* root, int level, myVector<T>& levels){
    if (root == nullptr) return 0;

    ++levels[level];
    getWidth(root->left, level + 1, levels);
    getWidth(root->right, level + 1, levels);
}

template <typename T>
int BinaryTree<T>::getHeight(Node<T>* root){
    if (root == nullptr) return 0;

    int left_height = getHeight(root->left);
    int right_height = getHeight(root->right);
    return ((left_height > right_height) ? left_height : right_height) + 1;
}