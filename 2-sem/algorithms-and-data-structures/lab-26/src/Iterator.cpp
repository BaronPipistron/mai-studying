#include "../include/Iterator.h"

template <typename T>
Iterator<T>::Iterator(Node<T>* node): node(node){}

template <typename T>
Node<T>* Iterator<T>::get_node(){
    return node;
}

template <typename T>
T& Iterator<T>::operator*(){
    return node->data;
}

template <typename T>
const T& Iterator<T>::operator*() const{
    return node->data;
}

template <typename T>
Iterator<T> Iterator<T>::operator++(){
    if (node != nullptr) node = node->next;

    return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator--(){
    if (node != nullptr) node = node->previous;

    return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator+(int64_t value){
    if (node == nullptr) return *this;

    if (value >= 0){
        while (node != nullptr && value != 0){
            --value;
            node = node->next;
        }
    } else{
        while (node != nullptr && value != 0){
            ++value;
            node = node->previous;
        }
    }

    return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator-(int64_t value){
    return *this + (-value);
}

template <typename T>
bool Iterator<T>::operator==(const Iterator<T>& other){
    return this->node == other.node;
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator<T>& other){
    return !(this->node == other.node);
}