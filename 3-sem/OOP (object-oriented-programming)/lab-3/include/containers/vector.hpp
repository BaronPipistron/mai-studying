#pragma once

#include <iostream>
#include <string>

template <typename T>
class vector final {
private:
    T* _arr;
    size_t _vec_size;
    size_t _vec_capacity;

public:
    vector();
    vector(const size_t, const T& = T());
    vector(const std::initializer_list<T>&);
    vector(const vector<T>&);
    vector(vector<T>&&) noexcept;
    virtual ~vector() noexcept;

    vector<T>& operator=(const vector<T>&);
    vector<T>& operator=(vector<T>&&) noexcept;

    void reserve(const size_t);
    void resize(const size_t, const T& = T());
    void shrink_to_fit();
    void clear();

    void push_back(const T&);
    template <typename ...Args>
    void emplace_back(const Args&...);
    void pop_back();

    void erase(size_t);

    T& operator[](size_t);
    T& at(size_t);
    T& front();
    T& back();
    const T& operator[](size_t) const;
    const T& at(size_t) const;
    const T& front() const;
    const T& back() const;

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    bool operator==(const vector<T>&) const;
    bool operator!=(const vector<T>&) const;
};
