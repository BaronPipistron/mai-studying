#include "../include/Iterator.h"

template <typename T>
Iterator<T>::Iterator(Node<T>* node): node(node){}

template <typename T>
Iterator<T>::Iterator(const List<T>& lst){
    node = lst.barrier;
}

template <typename T>
Iterator<T>& Iterator<T>::operator++(){
    if (node != nullptr){
        node = node->get_next;
    }

    return *this;
}

template <typename T>
Iterator<T>& Iterator<T>::operator+(size_t value){
    while (value != 0 && node != nullptr){
        node = node->get_next;
        --value;
    }

    return *this;
}

template <typename T>
T& Iterator<T>::operator*(){
    return node->get_val;
}

template <typename T>
const T& Iterator<T>::operator*() const{
    return node->get_val;
}

template <typename T>
bool Iterator<T>::operator==(const Iterator<T>& other) const{
    return this->node == other.node;
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator<T>& other) const{
    return !(this->node == other.node);
}