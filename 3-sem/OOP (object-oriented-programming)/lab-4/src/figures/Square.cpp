#include "../../include/figures/Square.h"

using namespace figures;

template <class T>
Square<T>::Square(const Point<T>& left_bottom, const Point<T>& right_bottom,
                  const Point<T>& left_top, const Point<T>& right_top) {

    if (check_square(left_bottom, right_bottom, left_top, right_top) == false) {
        throw std::invalid_argument("Given point<T>s do not form a square");
    }

    this->_left_bottom = left_bottom;
    this->_right_bottom = right_bottom;
    this->_left_top = left_top;
    this->_right_top = right_top;

    this->_side = sqrt(
                       (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) * 
                       (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) +
                       (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()) * 
                       (_right_bottom.get_y_cord() - _left_bottom.get_y_cord())
                       );
}

template <class T>
Square<T>::Square(const T& side) {
    if (side <= 0) {
        throw std::invalid_argument("Side of square must be more than zero");
    }

    this->_left_bottom = Point<T>(0, 0);
    this->_right_bottom = Point<T>(side, 0);
    this->_left_top = Point<T>(0, side);
    this->_right_top = Point<T>(side, side);

    this->_side = side;
}

template <class T>
Square<T>::Square(const Square<T>& other) noexcept
              : _left_bottom(other._left_bottom), _right_bottom(other._right_bottom),
                _left_top(other._left_top), _right_top(other._right_top),
                _side(other._side) 
{
}

template <class T>
Square<T>::Square(Square<T>&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_side = std::move(other._side);
}

template <class T>
Square<T>& Square<T>::operator=(const Square<T>& other) noexcept {
    if (this == &other) return *this;

    this->_left_bottom = other._left_bottom;
    this->_right_bottom = other._right_bottom;
    this->_left_top = other._left_top;
    this->_right_top = other._right_top;

    this->_side = other._side;
    return *this;
}

template <class T>
Square<T>& Square<T>::operator=(Square<T>&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_side = std::move(other._side);
    return *this;
}

template <class T>
figures::Point<T> Square<T>::calculate_geometric_center() const noexcept {
    Point<T> geometric_center ((_right_top.get_x_cord() + _left_bottom.get_x_cord()) / 2,
                               (_right_top.get_y_cord() + _left_bottom.get_y_cord()) / 2);
    return geometric_center;
}

template <class T>
T Square<T>::calculate_perimeter() const noexcept {
    return (_side * 4);
}

template <class T>
T Square<T>::calculate_area() const noexcept {
    return (_side * _side);
}

template <class T>
const T& Square<T>::get_side() const noexcept {
    return _side;
}

template <class T>
Square<T>::operator double() const {
    return (_side * _side);
}

template <class T>
bool Square<T>::operator==(const Square& other) const {
    return this->_left_bottom == other._left_bottom &&
           this->_right_bottom == other._right_bottom &&
           this->_left_top == other._left_top &&
           this->_right_top == other._right_top;
}

template <class T>
bool Square<T>::operator!=(const Square& other) const {
    return !(*this == other);
}

template <class T>
bool Square<T>::check_square(const Point<T>& first, const Point<T>& second, 
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

    double length = sqrt(
                         (second.get_x_cord() - first.get_x_cord()) * 
                         (second.get_x_cord() - first.get_x_cord()) +
                         (second.get_y_cord() - first.get_y_cord()) * 
                         (second.get_y_cord() - first.get_y_cord())
                         );

    double width = sqrt(
                        (third.get_x_cord() - first.get_x_cord()) * 
                        (third.get_x_cord() - first.get_x_cord()) +
                        (third.get_y_cord() - first.get_y_cord()) * 
                        (third.get_y_cord() - first.get_y_cord())
                        );

    bool compare_sides = (length == width);

    return (dd1 == dd2 && dd1 == dd3 && dd1 == dd4) && compare_sides;
}

namespace figures {

template <class U>
std::istream& operator>>(std::istream& is, Square<U>& square) {
    Point<U> left_bottom;
    Point<U> right_bottom;
    Point<U> left_top;
    Point<U> right_top;

    is >> left_bottom >> right_bottom >> left_top >> right_top;

    square = Square(left_bottom, right_bottom, left_top, right_top);
    return is; 
}

template <class U>
std::ostream& operator<<(std::ostream& os, const Square<U>& square) {
    os << "Left bottom point<T> coordinates: " << square._left_bottom << std::endl;
    os << "Right bottom point<T> coordinates: " << square._right_bottom << std::endl;
    os << "Left top point<T> coordinates: " << square._left_top << std::endl;
    os << "Right top point<T> coordinates: " << square._right_top << std::endl;
    os << "Side of square<T>: " << square._side << std::endl; 

    return os;
}

}; // namespace figures