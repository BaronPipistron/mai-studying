#include "../include/Node.h"

template <typename T>
Node<T>::Node(): data(0){}

template <typename T>
Node<T>::Node(const T& value): data(value){}

template <typename T>
Node<T>::Node(const T& value, Node<T>* previous, Node<T>* current): data(value), next(current), previous(previous){
    if (previous != nullptr){
        previous->next = this;
    }

    if (current != nullptr){
        current->previous = this;
    }
}