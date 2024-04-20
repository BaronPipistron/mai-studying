#include "../../include/figures/Rectangle.h"

using namespace figures;

template <class T>
Rectangle<T>::Rectangle(const Point<T>& left_bottom, const Point<T>& right_bottom,
                        const Point<T>& left_top, const Point<T>& right_top) {

    if (check_rectangle(left_bottom, right_bottom, left_top, right_top) == false) {
        throw std::invalid_argument("Given points do not form a rectangle");
    }

    this->_left_bottom = left_bottom;
    this->_right_bottom = right_bottom;
    this->_left_top = left_top;
    this->_right_top = right_top;

    this->_length = sqrt(
                         (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) * 
                         (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) +
                         (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()) * 
                         (_right_bottom.get_y_cord() - _left_bottom.get_y_cord())
                         );

    this->_width = sqrt(
                        (_left_top.get_x_cord() - _left_bottom.get_x_cord()) * 
                        (_left_top.get_x_cord() - _left_bottom.get_x_cord()) +
                        (_left_top.get_y_cord() - _left_bottom.get_y_cord()) * 
                        (_left_top.get_y_cord() - _left_bottom.get_y_cord())
                        );
}

template <class T>
Rectangle<T>::Rectangle(const T& length, const T& width) {
    if (length <= 0 || width <= 0) {
        throw std::invalid_argument("Length and width of rectangle must be more than zero");
    }

    this->_left_bottom = Point<T>(0, 0);
    this->_right_bottom = Point<T>(length, 0);
    this->_left_top = Point<T>(0, width);
    this->_right_top = Point<T>(length, width);

    this->_length = length;
    this->_width = width;
}

template <class T>
Rectangle<T>::Rectangle(const Rectangle<T>& other) noexcept 
                    : _left_bottom(other._left_bottom), _right_bottom(other._right_bottom),
                      _left_top(other._left_top), _right_top(other._right_top),
                      _length(other._length), _width(other._width) 
{
}

template <class T>
Rectangle<T>::Rectangle(Rectangle<T>&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_length = std::move(other._length);
    this->_width = std::move(other._width);
}

template <class T>
Rectangle<T>& Rectangle<T>::operator=(const Rectangle<T>& other) noexcept {
    if (this == &other) return *this;

    this->_left_bottom = other._left_bottom;
    this->_right_bottom = other._right_bottom;
    this->_left_top = other._left_top;
    this->_right_top = other._right_top;

    this->_length = other._length;
    this->_width = other._width;
    return *this;
}

template <class T>
Rectangle<T>& Rectangle<T>::operator=(Rectangle<T>&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_length = std::move(other._length);
    this->_width = std::move(other._width);
    return *this;
}

template <class T>
figures::Point<T> Rectangle<T>::calculate_geometric_center() const noexcept {
    Point<T> geometric_center ((_right_top.get_x_cord() + _left_bottom.get_x_cord()) / 2,
                               (_right_top.get_y_cord() + _left_bottom.get_y_cord()) / 2);
    return geometric_center;
}

template <class T>
T Rectangle<T>::calculate_perimeter() const noexcept {
    return (_length + _width) * 2;
}

template <class T>
T Rectangle<T>::calculate_area() const noexcept {
    return (_length * _width);
}

template <class T>
const T& Rectangle<T>::get_length() const noexcept {
    return _length;
}

template <class T>
const T& Rectangle<T>::get_width() const noexcept {
    return _width;
}

template <class T>
Rectangle<T>::operator double() const {
    return (_length * _width);
}

template <class T>
bool Rectangle<T>::operator==(const Rectangle<T>& other) const {
    return this->_left_bottom == other._left_bottom &&
           this->_right_bottom == other._right_bottom &&
           this->_left_top == other._left_top &&
           this->_right_top == other._right_top;
}

template <class T>
bool Rectangle<T>::operator!=(const Rectangle<T>& other) const {
    return !(*this == other);
}

template <class T>
bool Rectangle<T>::check_rectangle(const Point<T>& first, const Point<T>& second, 
                                const Point<T>& third, const Point<T>& fourth) const noexcept {

    double c_x = (
                  first.get_x_cord() + second.get_x_cord() + 
                  third.get_x_cord() + fourth.get_x_cord()
                  ) / 4;

    double c_y = (
                  first.get_y_cord() + second.get_y_cord() + 
                  third.get_y_cord() + fourth.get_y_cord()
                  ) / 4;

    double dd1 = (c_x - first.get_x_cord()) * (c_x - first.get_x_cord()) + 
                 (c_y - first.get_y_cord()) * (c_y - first.get_y_cord());

    double dd2 = (c_x - second.get_x_cord()) * (c_x - second.get_x_cord()) + 
                 (c_y - second.get_y_cord()) * (c_y - second.get_y_cord());

    double dd3 = (c_x - third.get_x_cord()) * (c_x - third.get_x_cord()) + 
                 (c_y - third.get_y_cord()) * (c_y - third.get_y_cord());

    double dd4 = (c_x - fourth.get_x_cord()) * (c_x - fourth.get_x_cord()) + 
                 (c_y - fourth.get_y_cord()) * (c_y - fourth.get_y_cord());

    return (dd1 == dd2 && dd1 == dd3 && dd1 == dd4);
}

namespace figures {

template <class U>
std::istream& operator>>(std::istream& is, Rectangle<U>& rectangle) {
    Point<U> left_bottom;
    Point<U> right_bottom;
    Point<U> left_top;
    Point<U> right_top;

    is >> left_bottom >> right_bottom >> left_top >> right_top;

    rectangle = Rectangle(left_bottom, right_bottom, left_top, right_top);
    return is; 
}

template <class U>
std::ostream& operator<<(std::ostream& os, const Rectangle<U>& rectangle) {
    os << "Left bottom point<T> coordinates: " << rectangle._left_bottom << std::endl;
    os << "Right bottom point<T> coordinates: " << rectangle._right_bottom << std::endl;
    os << "Left top point<T> coordinates: " << rectangle._left_top << std::endl;
    os << "Right top point<T> coordinates: " << rectangle._right_top << std::endl;
    os << "Length of rectangle<T>: " << rectangle._length << std::endl;
    os << "Width of rectangle<T>: " << rectangle._width << std::endl;

    return os;
}

}; // namespace figures