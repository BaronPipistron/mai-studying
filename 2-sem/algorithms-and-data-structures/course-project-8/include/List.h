#ifndef LIST_HPP
#define LIST_HPP

#include "Node.h"
#include "Iterator.h"
#include <iostream>

template <typename T>
class Iterator;

template <typename T>
class List{
    friend class Iterator<T>;

private:
    Node<T>* barrier;
    size_t list_size;

public:
    List();
    ~List();

    Iterator<T> begin() const;
    Iterator<T> end() const;

    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front();
    void pop_back();
    void insert(const Iterator<T>& it, const T& value);
    void erase(const Iterator<T>& start, const Iterator<T>& end);

    template <typename... Args>
    void emplace_front(const Args&... args);

    template <typename... Args>
    void emplace_back(const Args&... args);

    void swap_halfs();
    std::ostream& operator<<(std::ostream& os);
};

#include "../src/List.cpp"

#endif