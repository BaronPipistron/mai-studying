#ifndef MYVECTORHPP
#define MYVECTORHPP

#include <iostream>

template <typename T>
class myVector{
private:
    T* data;
    size_t vec_size;
    size_t vec_capacity;

public:
    myVector();
    myVector(const myVector<T>& another_vector);
    myVector(const std::initializer_list<T>& list);
    ~myVector();

    void reserve(size_t n);
    void resize(size_t new_size, const T& value = T());
    void shrink_to_fit();
    void push_back(const T& value);
    void pop_back();
    T& at(size_t index);
    const T& front() const;
    const T& back() const;
    bool empty() const;
    size_t size() const;
    size_t capacity() const;
    void clear();

    template <typename... Args>
    void emplace_back(const Args& ...args);

    T& operator[](size_t i) const;
    bool operator==(const myVector<T>& another_vector);
    bool operator!=(const myVector<T>& another_vector);
    void erase(size_t element_index);
    void erase(size_t start_index, size_t end_index);
    void swap(myVector<T>& another_vector);
};

#include "../src/MyVector.cpp"

#endif