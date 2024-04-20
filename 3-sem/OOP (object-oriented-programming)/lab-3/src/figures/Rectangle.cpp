#include "../../include/figures/Rectangle.hpp"

using namespace figures;

Rectangle::Rectangle(const Point& left_bottom, const Point& right_bottom,
                     const Point& left_top, const Point& right_top) {

    if (check_rectangle(left_bottom, right_bottom, left_top, right_top) == false) {
        throw std::invalid_argument("Given points do not form a rectangle");
    }

    this->_left_bottom = left_bottom;
    this->_right_bottom = right_bottom;
    this->_left_top = left_top;
    this->_right_top = right_top;

    this->_length = sqrt((_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) * (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) +
                         (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()) * (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()));

    this->_width = sqrt((_left_top.get_x_cord() - _left_bottom.get_x_cord()) * (_left_top.get_x_cord() - _left_bottom.get_x_cord()) +
                        (_left_top.get_y_cord() - _left_bottom.get_y_cord()) * (_left_top.get_y_cord() - _left_bottom.get_y_cord()));
}

Rectangle::Rectangle(const double length, const double width) {
    if (length <= 0 || width <= 0) {
        throw std::invalid_argument("Length and width of rectangle must be more than zero");
    }

    this->_left_bottom = Point(0, 0);
    this->_right_bottom = Point(length, 0);
    this->_left_top = Point(0, width);
    this->_right_top = Point(length, width);

    this->_length = length;
    this->_width = width;
}

Rectangle::Rectangle(const Rectangle& other) noexcept 
                    : _left_bottom(other._left_bottom), _right_bottom(other._right_bottom),
                      _left_top(other._left_top), _right_top(other._right_top),
                      _length(other._length), _width(other._width) {}

Rectangle::Rectangle(Rectangle&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_length = std::move(other._length);
    this->_width = std::move(other._width);
}

Rectangle& Rectangle::operator=(const Rectangle& other) noexcept {
    if (this == &other) return *this;

    this->_left_bottom = other._left_bottom;
    this->_right_bottom = other._right_bottom;
    this->_left_top = other._left_top;
    this->_right_top = other._right_top;

    this->_length = other._length;
    this->_width = other._width;
    return *this;
}

Rectangle& Rectangle::operator=(Rectangle&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_length = std::move(other._length);
    this->_width = std::move(other._width);
    return *this;
}

figures::Point Rectangle::calculate_geometric_center() const noexcept {
    Point geometric_center ((_right_top.get_x_cord() + _left_bottom.get_x_cord()) / 2,
                            (_right_top.get_y_cord() + _left_bottom.get_y_cord()) / 2);
    return geometric_center;
}

double Rectangle::calculate_perimeter() const noexcept {
    return (_length + _width) * 2;
}

double Rectangle::calculate_area() const noexcept {
    return (_length * _width);
}

const double Rectangle::get_length() const noexcept {
    return _length;
}

const double Rectangle::get_width() const noexcept {
    return _width;
}

Rectangle::operator double() const {
    return (_length * _width);
}

bool Rectangle::operator==(const Rectangle& other) const {
    return this->_left_bottom == other._left_bottom &&
           this->_right_bottom == other._right_bottom &&
           this->_left_top == other._left_top &&
           this->_right_top == other._right_top;
}

bool Rectangle::operator!=(const Rectangle& other) const {
    return !(*this == other);
}

bool Rectangle::check_rectangle(const Point& first, const Point& second, 
                                const Point& third, const Point& fourth) const noexcept {

    double c_x = (first.get_x_cord() + second.get_x_cord() + third.get_x_cord() + fourth.get_x_cord()) / 4;
    double c_y = (first.get_y_cord() + second.get_y_cord() + third.get_y_cord() + fourth.get_y_cord()) / 4;

    double dd1 = (c_x - first.get_x_cord()) * (c_x - first.get_x_cord()) + (c_y - first.get_y_cord()) * (c_y - first.get_y_cord());
    double dd2 = (c_x - second.get_x_cord()) * (c_x - second.get_x_cord()) + (c_y - second.get_y_cord()) * (c_y - second.get_y_cord());
    double dd3 = (c_x - third.get_x_cord()) * (c_x - third.get_x_cord()) + (c_y - third.get_y_cord()) * (c_y - third.get_y_cord());
    double dd4 = (c_x - fourth.get_x_cord()) * (c_x - fourth.get_x_cord()) + (c_y - fourth.get_y_cord()) * (c_y - fourth.get_y_cord());

    return (dd1 == dd2 && dd1 == dd3 && dd1 == dd4);
}

namespace figures {

std::istream& operator>>(std::istream& is, Rectangle& rectangle) {
    Point left_bottom, right_bottom, left_top, right_top;
    is >> left_bottom >> right_bottom >> left_top >> right_top;
    rectangle = Rectangle(left_bottom, right_bottom, left_top, right_top);
    return is; 
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rectangle) {
    os << "Left bottom point coordinates: " << rectangle._left_bottom << std::endl;
    os << "Right bottom point coordinates: " << rectangle._right_bottom << std::endl;
    os << "Left top point coordinates: " << rectangle._left_top << std::endl;
    os << "Right top point coordinates: " << rectangle._right_top << std::endl;
    os << "Length of rectangle: " << rectangle._length << std::endl;
    os << "Width of rectangle: " << rectangle._width << std::endl;
    return os;
}

}; // namespace figures