#pragma once

#include <iostream>

namespace figures {

template <class T>
class Point final {
private:
    T _x;
    T _y;

public:
    Point() = default;
    Point(const T&, const T&) noexcept;
    Point(const std::initializer_list<T>&);
    Point(const Point<T>&) noexcept;
    Point(Point<T>&&) noexcept;

    Point& operator=(const Point<T>&) noexcept;
    Point& operator=(Point<T>&&) noexcept;

    const T& get_x_cord() const noexcept;
    const T& get_y_cord() const noexcept;

    void set_x_cord(const T&) noexcept;
    void set_y_cord(const T&) noexcept;

    bool operator==(const Point<T>& other) const noexcept;
    bool operator!=(const Point<T>& other) const noexcept;

    template <class U>
    friend std::istream& operator>>(std::istream&, Point<U>&);

    template <class U>
    friend std::ostream& operator<<(std::ostream&, const Point<U>&);

    virtual ~Point() = default;
};

}; // namespace figures