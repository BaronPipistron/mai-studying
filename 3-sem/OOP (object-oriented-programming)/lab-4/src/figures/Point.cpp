#include "../../include/figures/Point.h"

using namespace figures;

template <class T>
Point<T>::Point(const T& x, const T& y) noexcept :
    _x(x), _y(y) 
{
}

template <class T>
Point<T>::Point(const std::initializer_list<T>& list) {
    if (list.size() != 2) {
        throw std::invalid_argument("List must contains two coordinates (x, y)");
    }

    _x = *(list.begin());
    _y = *(list.end() - 1);
}

template <class T>
Point<T>::Point(const Point<T>& other) noexcept : 
    _x(other._x), _y(other._y) 
{
}

template <class T>
Point<T>::Point(Point<T>&& other) noexcept {
    this->_x = std::move(other._x);
    this->_y = std::move(other._y);
}

template <class T>
Point<T>& Point<T>::operator=(const Point<T>& other) noexcept {
    if (this == &other) return *this;

    this->_x = other._x;
    this->_y = other._y;
    return *this;
}

template <class T>
Point<T>& Point<T>::operator=(Point<T>&& other) noexcept {
    this->_x = std::move(other._x);
    this->_y = std::move(other._y);
    return *this;
}

template <class T>
const T& Point<T>::get_x_cord() const noexcept {
    return _x;
}

template <class T>
const T& Point<T>::get_y_cord() const noexcept {
    return _y;
}

template <class T>
void Point<T>::set_x_cord(const T& new_x) noexcept {
    if (this->_x == new_x) return;

    this->_x = new_x;
}

template <class T>
void Point<T>::set_y_cord(const T& new_y) noexcept {
    if (this->_y == new_y) return;

    this->_y = new_y;
}

template <class T>
bool Point<T>::operator==(const Point<T>& other) const noexcept {
    return (this->_x == other._x && this->_y == other._y);
}

template <class T>
bool Point<T>::operator!=(const Point<T>& other) const noexcept {
    return !(*this == other);
}

namespace figures {

    template <class U>
    std::istream& operator>>(std::istream& is, Point<U>& point) {
        is >> point._x >> point._y;
        return is;
    }

    template <class U>
    std::ostream& operator<<(std::ostream& os, const Point<U>& point) {
        os << point._x << " " << point._y;
        return os;
    }

};