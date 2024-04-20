#include "../../include/figures/Point.hpp"

using namespace figures;

Point::Point(double x, double y) noexcept : _x(x), _y(y) {}

Point::Point(const std::initializer_list<double>& list) {
    if (list.size() != 2) {
        throw std::invalid_argument("List must contains two coordinates (x, y)");
    }

    _x = *(list.begin());
    _y = *(list.end() - 1);
}

Point::Point(const Point& other) noexcept : _x(other._x), _y(other._y) {}

Point::Point(Point&& other) noexcept {
    this->_x = std::move(other._x);
    this->_y = std::move(other._y);
}

Point& Point::operator=(const Point& other) noexcept {
    if (this == &other) return *this;

    this->_x = other._x;
    this->_y = other._y;
    return *this;
}

Point& Point::operator=(Point&& other) noexcept {
    this->_x = std::move(other._x);
    this->_y = std::move(other._y);
    return *this;
}

const double Point::get_x_cord() const noexcept {
    return _x;
}

const double Point::get_y_cord() const noexcept {
    return _y;
}

void Point::set_x_cord(double new_x) noexcept {
    if (this->_x == new_x) return;

    this->_x = new_x;
}

void Point::set_y_cord(double new_y) noexcept {
    if (this->_y == new_y) return;

    this->_y = new_y;
}

bool Point::operator==(const Point& other) const noexcept {
    return (this->_x == other._x && this->_y == other._y);
}

bool Point::operator!=(const Point& other) const noexcept {
    return !(*this == other);
}

namespace figures {

    std::istream& operator>>(std::istream& is, Point& point) {
        is >> point._x >> point._y;
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << point._x << " " << point._y;
        return os;
    }

};