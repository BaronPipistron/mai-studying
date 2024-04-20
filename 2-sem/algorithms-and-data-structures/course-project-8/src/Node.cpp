#include "../include/Node.h"

template <typename T>
Node<T>::Node(const T& value): val(value){}

template <typename T>
Node<T>::Node(const T& value, Node<T>* next): val(value), next(next){}

template <typename T>
T& Node<T>::get_val() const{
    return val;
}

template <typename T>
Node<T>* Node<T>::get_next() const{
    return next;
}

template <typename T>
void Node<T>::set_val(const T& value){
    val = value;
}

template <typename T>
void Node<T>::set_next(Node<T>* next_node){
    next = next_node;
}