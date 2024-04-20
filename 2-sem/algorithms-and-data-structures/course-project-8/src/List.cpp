#include "../include/List.h"
#include <iostream>

template <typename T>
List<T>::List(){
    barrier = new Node<T>();
    barrier->next = barrier;
    list_size = 0;
}

template <typename T>
List<T>::~List(){
    Iterator<T> cur = ++begin();
    Iterator<T> to_delet = cur;

    while (cur.node != barrier){
        ++cur;
        delete to_delet.node;
        to_delet = cur;
    }

    delete cur.node;
}

template <typename T>
Iterator<T> List<T>::begin() const{
    return Iterator<T>(*this);
}

template <typename T>
Iterator<T> List<T>::end() const{
    Node<T>* end = barrier;

    while (end->next != barrier){
        end = end->next;
    }
    return Iterator<T>(end);
}

template <typename T>
void List<T>::push_front(const T& value){
    if (begin().node->next == barrier){
        begin().node->next = new Node<T>(value, barrier);
        barrier = begin().node->next;

        ++list_size;
        return;
    }

    begin().node->next = new Node<T>(*(begin()), begin().node->next);
    *(begin()) = value;
    ++list_size;
}

template <typename T>
void List<T>::push_back(const T& value){
    if (begin() == end()){
        begin().node->next = new Node<T>(value, barrier);
        barrier = begin().node->next;

        ++list_size;
        return;
    }

    Iterator<T> iterator = begin();
    while (iterator.node->next->next != barrier){
        ++iterator;
    }

    iterator.node->next = new Node<T>(value, iterator.node->next);
    ++list_size;
}

template <typename T>
void List<T>::pop_front(){
    if (begin().node->next == barrier) return;

    Iterator<T> it = ++begin();
    begin().node->val = (++begin()).node->val;
    begin().node->next = (++begin()).node->next;

    delete it.node;
    --list_size;
}

template <typename T>
void List<T>::pop_back(){
    if (begin().node->next == barrier) return;

    Iterator<T> it = ++end();
    while (it.node->next->next != barrier){
        ++it;
    }
    
    delete it.node->next;
    it.node->next = barrier;
    --list_size;
}

template <typename T>
void List<T>::insert(const Iterator<T>& it, const T& value){
    it.node->next = new Node<T>(value, it.node->next);
    ++list_size;
}

template <typename T>
void List<T>::erase(const Iterator<T>& start, const Iterator<T>& end){
    Iterator<T> it = start;
    ++it;
    start.node->next = end.node;

    while (it != end){
        if (it == begin()) start = end.node;

        Node<T>* to_delet = it.node;
        ++it;
        delete to_delet;
        --list_size;
    }
}

template <typename T>
template <typename... Args>
void List<T>::emplace_front(const Args&... args){
    push_front(T(args...));
}

template <typename T>
template <typename... Args>
void List<T>::emplace_back(const Args&... args){
    push_back(T(args...));
}

template <typename T>
void List<T>::swap_halfs(){
    if (barrier == nullptr) return;

    Iterator<T> first_half = begin();
    Iterator<T> second_half = begin();
    for (size_t i = 0; i != list_size / 2; ++i){
        ++second_half;
    }

    while (second_half.node->next != barrier){
        T swap = first_half.node->val;
        first_half.node->val = second_half.node->val;
        second_half.node->val = swap;

        ++first_half;
        ++second_half;
    }
}

template <typename T>
std::ostream& List<T>::operator<<(std::ostream& os){
    Node<T>* cur = barrier;
    while (cur->next != barrier){
        os << cur->val << " ";
        cur = cur->next;
    }

    return os;
}