#include "../include/ExpressionTree.h"

ExpressionTree::ExpressionTree(): root(nullptr){}

ExpressionTree::ExpressionTree(const std::string& expression){
    std::string postfix = doPostfix(expression);
    root = createTree(postfix);
}

ExpressionTree::~ExpressionTree(){
    deleteTree(root);
}

Node* ExpressionTree::getRoot() const{
    return this->root;
}

size_t getPriority(char c){
    if (c == '+' || c == '-'){
        return 1;
    } else if (c == '*' || c == '/'){
        return 2;
    } else if (c == '^'){
        return 3;
    } else if (c == '~'){
        return 4;
    }

    return 0;
}

bool isOperator(char c){
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

std::string ExpressionTree::doPostfix(const std::string& expression){
    std::string postfix = "";
    Stack<char> stack;

    for (size_t i = 0; i != expression.size(); ++i){
        char c = expression[i];

        if (!isOperator(c) && c != '(' && c != ')'){
            postfix += c;
        } else if (c == '('){
            stack.push(c);
        } else if (c == ')'){
            while (stack.top() != '('){
                postfix += stack.top();
                stack.pop();
            }
            stack.pop();
        } else{
            if (c == '-' && expression[i - 1] == '(') c = '~';

            while (!stack.empty() && (getPriority(stack.top()) >= getPriority(c))){
                postfix += stack.top();
                stack.pop();
            }
            stack.push(c);
        }
    }

    while (!stack.empty()){
        postfix += stack.top();
        stack.pop();
    }
    return postfix;
}

void ExpressionTree::deleteTree(Node* node){
    if (node == nullptr) return;

    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

Node* ExpressionTree::createTree(const std::string& postfix){
    if (postfix.length() == 0) return nullptr;

    Stack<Node*> stack;

    for (char c: postfix){
        if (c == '~'){
            Node* node_x = stack.top();
            stack.pop();

            Node* node = new Node('-', nullptr, node_x);
            stack.push(node);
        } else if (isOperator(c)){
            Node* node_x = stack.top();
            stack.pop();

            Node* node_y = stack.top();
            stack.pop();

            Node* node = new Node(c, node_y, node_x);
            stack.push(node);
        } else{
            stack.push(new Node(c));
        }
    }

    return stack.top();
}

void ExpressionTree::printPostfix(Node* root) const{
    if (root == nullptr) return;

    printPostfix(root->left);
    printPostfix(root->right);
    std::cout << root->data;
}

void ExpressionTree::printInfix(Node* root) const{
    if (root == nullptr) return;

    if (isOperator(root->data)){
        std::cout << '(';
    }

    printInfix(root->left);
    std::cout << root->data;
    printInfix(root->right);

    if (isOperator(root->data)){
        std::cout << ')';
    }
}

void ExpressionTree::printTree(Node* root, const size_t height) const{
    if (root != nullptr){
        printTree(root->right, height + 1);
        for (size_t i = 0; i < height; ++i){
            std::cout << "\t";
        }
        std::cout << root->data << "\n";
        printTree(root->left, height + 1);
    }
}

void ExpressionTree::replace(char var, const std::string& expression){
    Node* expression_tree = createTree(doPostfix(expression));
    root = replace(root, var, expression_tree);
}

Node* ExpressionTree::replace(Node* root, char var, Node* expression_tree){
    if (root == nullptr) return root;

    if (root->data == var){
        Node* new_node = new Node(*expression_tree);
        delete root;
        root = new_node;
        return root;
    }

    root->left = replace(root->left, var, expression_tree);
    root->right = replace(root->right, var, expression_tree);
    return root;
}