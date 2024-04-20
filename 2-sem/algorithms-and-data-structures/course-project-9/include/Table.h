#ifndef TABLE_HPP
#define TABLE_HPP

#include "../../course-project-7/include/MyVector.h"
#include "Line.h"

#include <iostream>

template <typename T>
class Table{
private:
    myVector<Line<T>> lines;
    size_t winner(const size_t pos_a, const size_t pos_b, const size_t n, myVector<size_t>& win_match, myVector<T>& lines);

public:
    Table() = default;
    ~Table() = default;
    myVector<Line<T>>& get_lines() const;

    void push_back(const T& key, const std::string& data);
    Line<T> search(const T& key) const;
    void sort(const T& inf);
    void print() const;

};

#include "../src/Table.cpp"

#endif