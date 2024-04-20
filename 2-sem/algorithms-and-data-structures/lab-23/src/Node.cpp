#include "../include/Node.h"

template <typename T>
Node<T>::Node(): data(0), left(nullptr), right(nullptr){}

template <typename T>
Node<T>::Node(const T& value): data(value), left(nullptr), right(nullptr){}

template <typename T>
Node<T>::Node(const T& value, Node<T>* left, Node<T>* right):
        data(value), left(left), right(right){}