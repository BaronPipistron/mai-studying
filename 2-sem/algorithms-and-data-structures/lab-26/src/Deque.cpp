#include "../include/Deque.h"

template <typename T>
Deque<T>::Deque(): head(nullptr), tail(nullptr), sz(0){}

template <typename T>
Deque<T>::~Deque(){
    if (begin().node == nullptr) return;

    Iterator<T> cur = begin();
    Iterator<T> to_delet = cur;

    while (cur.node != nullptr){
        ++cur;
        delete to_delet.node;
        to_delet = cur;
    }

    delete cur.node;
}

template <typename T>
Iterator<T> Deque<T>::begin() const{
    Iterator<T> tmp(head);
    return tmp;
}

template <typename T>
Iterator<T> Deque<T>::end() const{
    Iterator<T> tmp(tail);
    return tmp;
}

template <typename T>
void Deque<T>::push_front(const T& value){
    if (begin().node == nullptr){
        head = new Node<T>(value);
        tail = head;
        ++sz;
        return;
    }

    head = new Node<T>(value, nullptr, head);
    ++sz;

    return;
}

template <typename T>
void Deque<T>::push_back(const T& value){
    if (begin().node == nullptr){
        head = new Node<T>(value);
        tail = head;
        ++sz;
        return;
    }

    tail = new Node<T>(value, tail, nullptr);
    ++sz;
    return;
}

template <typename T>
void Deque<T>::pop_front(){
    if (sz == 0) return;

    Node<T>* tmp = head;
    head = head->next;
    delete tmp;
    --sz;
}

template <typename T>
void Deque<T>::pop_back(){
    if (sz == 0) return;

    Node<T>* tmp = tail;
    tail = tail->previous;
    delete tmp;
    --sz;
}

template <typename T>
size_t Deque<T>::size() const{
    return sz;
}

template <typename T>
bool Deque<T>::empty() const{
    return sz == 0;
}

template <typename T>
void Deque<T>::clear(){
    Iterator<T> cur = begin();
    Iterator<T> to_delet = cur;

    while (cur.node != nullptr){
        ++cur;
        delete to_delet.node;
        to_delet = cur;
    }

    delete cur.node;
}

template <typename T>
void Deque<T>::sort(){
    if (sz == 0) return;

    Node<T>* max;
    for (Node<T>* i = begin().node; i != end().node->next; i = i->next){
        max = i;
        for (Node<T>* j = i->next; j != end().node->next; j = j->next){
            if (max->data < j->data){
                max = j;
            }
        }

        head = new Node<T>(max->data, nullptr, head);

        if (max == tail){
            tail = tail->previous;
            tail->next = nullptr;
        } else{
            max->previous->next = max->next;
            max->next->previous = max->previous;
        }

        if (i != max) i = i->previous;
    }

    delete max;
}

template <typename T>
void Deque<T>::print() const{
    if (sz == 0){
        std::cout << "Deque is empty\n";
        return;
    }

    std::cout << "Deque = { ";
    
    for (Iterator<T> i = begin(); i != end() + 1; ++i){
        std::cout << i.node->data << " ";
    }

    std::cout << "}\n";
}