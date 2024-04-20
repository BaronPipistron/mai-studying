#include <iostream>

#include "../include/BinaryTree.h"

int main(){
    BinaryTree<int> bin_tree(10);
    bin_tree.insertNode(15, bin_tree.getRoot());
    bin_tree.insertNode(8, bin_tree.getRoot());
    bin_tree.insertNode(5, bin_tree.getRoot());
    bin_tree.insertNode(4, bin_tree.getRoot());
    bin_tree.insertNode(3, bin_tree.getRoot());
    bin_tree.insertNode(17, bin_tree.getRoot());
    bin_tree.insertNode(14, bin_tree.getRoot());
    bin_tree.insertNode(13, bin_tree.getRoot());
    bin_tree.insertNode(16, bin_tree.getRoot());
    bin_tree.insertNode(18, bin_tree.getRoot());
    bin_tree.insertNode(6, bin_tree.getRoot());

    bin_tree.removeNode(15, bin_tree.getRoot());

    bin_tree.visualize(bin_tree.getRoot());
    std::cout << "\nWidth of tree: " << bin_tree.getMaxWidth(bin_tree.getRoot()) << "\n";

    return 0;
}