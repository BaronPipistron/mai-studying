#ifndef LINE_HPP
#define LINE_HPP

#include <string>

template <typename T>
class Table;

template <typename T>
class Line{
    friend class Table<T>;
private:
    T key;
    std::string data;

public:
    Line() = default;
    Line(const T& key, const std::string& data);
    ~Line() = default;

    T get_key();
    std::string get_data() const;

    bool operator<(const Line<T>& other) const;
};

#include "../src/Line.cpp"

#endif