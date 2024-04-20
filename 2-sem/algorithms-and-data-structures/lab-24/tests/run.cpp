#include <iostream>
#include <string>

#include "../include/ExpressionTree.h"

int main(){
    std::string expr = "2*1+4*5/7";
    ExpressionTree tree(expr);

    tree.printInfix(tree.getRoot());
    std::cout << "\n";
    tree.printPostfix(tree.getRoot());
    std::cout << "\n";
    tree.printTree(tree.getRoot());
    std::cout << "\n\n\n";
    tree.replace('a', "i+4");
    tree.printTree(tree.getRoot());
}