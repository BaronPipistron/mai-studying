#include "../../include/figures/Trapeze.h"

#define EPS 0.000000000001

using namespace figures;

template <class T>
Trapeze<T>::Trapeze(const Point<T>& left_bottom, const Point<T>& right_bottom,
                    const Point<T>& left_top, const Point<T>& right_top) {

    std::pair<bool, bool> trapeze_check = check_trapeze(
                                                        left_bottom, 
                                                        right_bottom, 
                                                        left_top, 
                                                        right_top
                                                        );

    if (trapeze_check.first == false) {
        throw std::invalid_argument("Given point<T>s do not form a trapeze");
    }

    this->_left_bottom = left_bottom;
    this->_right_bottom = right_bottom;
    this->_left_top = left_top;
    this->_right_top = right_top;

    if (trapeze_check.second == true) {
        this->_bottom_base = sqrt(
                                  (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) * 
                                  (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) +
                                  (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()) * 
                                  (_right_bottom.get_y_cord() - _left_bottom.get_y_cord())
                                  );
    
        this->_top_base = sqrt(
                               (_right_top.get_x_cord() - _left_top.get_x_cord()) * 
                               (_right_top.get_x_cord() - _left_top.get_x_cord()) +
                               (_right_top.get_y_cord() - _left_top.get_y_cord()) * 
                               (_right_top.get_y_cord() - _left_top.get_y_cord())
                               );
    
        Point<T> bottom_base_vec (
                                  _right_bottom.get_x_cord() - _left_bottom.get_x_cord(), 
                                  _right_bottom.get_y_cord() - _left_bottom.get_y_cord()
                                  );

        Point<T> top_base_vec (
                               _right_top.get_x_cord() - _left_top.get_x_cord(), 
                               _right_top.get_y_cord() - _left_top.get_y_cord()
                               );

        this->_height = sqrt(
                             (top_base_vec.get_x_cord() - bottom_base_vec.get_x_cord()) *
                             (top_base_vec.get_x_cord() - bottom_base_vec.get_x_cord()) +
                             (top_base_vec.get_y_cord() - bottom_base_vec.get_y_cord()) * 
                             (top_base_vec.get_y_cord() - bottom_base_vec.get_y_cord())
                             );

    } else {
        this->_bottom_base = sqrt(
                                  (_left_top.get_x_cord() - _left_bottom.get_x_cord()) * 
                                  (_left_top.get_x_cord() - _left_bottom.get_x_cord()) +
                                  (_left_top.get_y_cord() - _left_bottom.get_y_cord()) * 
                                  (_left_top.get_y_cord() - _left_bottom.get_y_cord())
                                  );

        this->_top_base = sqrt(
                               (_right_top.get_x_cord() - _right_bottom.get_x_cord()) * 
                               (_right_top.get_x_cord() - _right_bottom.get_x_cord()) +
                               (_right_top.get_y_cord() - _right_bottom.get_y_cord()) * 
                               (_right_top.get_y_cord() - _right_bottom.get_y_cord())
                               );
    
        Point<T> bottom_base_vec (
                                  _left_top.get_x_cord() - _left_bottom.get_x_cord(), 
                                  _left_top.get_y_cord() - _left_bottom.get_y_cord()
                                  );

        Point<T> top_base_vec (
                               _right_top.get_x_cord() - _right_bottom.get_x_cord(), 
                               _right_top.get_y_cord() - _right_bottom.get_y_cord()
                               );

        this->_height = sqrt(
                             (top_base_vec.get_x_cord() - bottom_base_vec.get_x_cord()) * 
                             (top_base_vec.get_x_cord() - bottom_base_vec.get_x_cord()) +
                             (top_base_vec.get_y_cord() - bottom_base_vec.get_y_cord()) * 
                             (top_base_vec.get_y_cord() - bottom_base_vec.get_y_cord())
                             );
    }
}

template <class T>
Trapeze<T>::Trapeze(const T& bottom_base, const T& top_base, const T& height) {
    if (bottom_base <= 0 || top_base <= 0 || height <= 0) {
        throw std::invalid_argument("Bottom base, top base and height of trapeze must be more than zero");
    }

    this->_left_bottom = Point<T>(0, 0);
    this->_right_bottom = Point<T>(bottom_base, 0);
    this->_left_top = Point<T>(0, height);
    this->_right_top = Point<T>(top_base, height);

    this->_bottom_base = bottom_base;
    this->_top_base = top_base;
    this->_height = height;
}

template <class T>
Trapeze<T>::Trapeze(const Trapeze<T>& other) noexcept 
                : _left_bottom(other._left_bottom), _right_bottom(other._right_bottom),
                  _left_top(other._left_top), _right_top(other._right_top),
                  _bottom_base(other._bottom_base), _top_base(other._top_base), _height(other._height) 
{
}

template <class T>
Trapeze<T>::Trapeze(Trapeze<T>&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_bottom_base = std::move(other._bottom_base);
    this->_top_base = std::move(other._top_base);
    this->_height = std::move(other._height);
}

template <class T>
Trapeze<T>& Trapeze<T>::operator=(const Trapeze<T>& other) noexcept {
    if (this == &other) return *this;

    this->_left_bottom = other._left_bottom;
    this->_right_bottom = other._right_bottom;
    this->_left_top = other._left_top;
    this->_right_top = other._right_top;

    this->_bottom_base = other._bottom_base;
    this->_top_base = other._top_base;
    this->_height = other._height;
    return *this;
}

template <class T>
Trapeze<T>& Trapeze<T>::operator=(Trapeze<T>&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_bottom_base = std::move(other._bottom_base);
    this->_top_base = std::move(other._top_base);
    this->_height = std::move(other._height);
    return *this;
}

template <class T>
figures::Point<T> Trapeze<T>::calculate_geometric_center() const noexcept {
    Point<T> horizontal_bottom (
                                _right_bottom.get_x_cord() - _left_bottom.get_x_cord(), 
                                _right_bottom.get_y_cord() - _left_bottom.get_y_cord()
                                );

    Point<T> horizontal_top (
                             _right_top.get_x_cord() - _left_top.get_x_cord(), 
                             _right_top.get_y_cord() - _left_top.get_y_cord()
                             );

    T cos_between_bottom_top = (horizontal_bottom.get_x_cord() * horizontal_top.get_x_cord() + 
                                horizontal_bottom.get_y_cord() * horizontal_top.get_y_cord()) / 
                               (sqrt(
                                     horizontal_bottom.get_x_cord() * 
                                     horizontal_bottom.get_x_cord() + 
                                     horizontal_bottom.get_y_cord() * 
                                     horizontal_bottom.get_y_cord()
                                     ) * 
                                sqrt(
                                     horizontal_top.get_x_cord() * 
                                     horizontal_top.get_x_cord() + 
                                     horizontal_top.get_y_cord() * 
                                     horizontal_top.get_y_cord()
                                     )
                                );

    if (cos_between_bottom_top == 1) {
        Point<T> bottom_base_middle (
                                     (_left_bottom.get_x_cord() + _right_bottom.get_x_cord()) / 2, 
                                     (_left_bottom.get_y_cord() + _right_bottom.get_y_cord()) / 2
                                     );

        Point<T> top_base_middle (
                                  (_left_top.get_x_cord() + _right_top.get_x_cord()) / 2, 
                                  (_left_top.get_y_cord() + _right_top.get_y_cord()) / 2
                                  );

        Point<T> geometric_center (
                                   (bottom_base_middle.get_x_cord() + top_base_middle.get_x_cord()) / 2, 
                                   (bottom_base_middle.get_y_cord() + top_base_middle.get_y_cord()) / 2
                                   );
        return geometric_center;
    } else {
        Point<T> bottom_base_middle (
                                     (_left_bottom.get_x_cord() + _left_top.get_x_cord()) / 2, 
                                     (_left_bottom.get_y_cord() + _left_top.get_y_cord()) / 2
                                     );

        Point<T> top_base_middle (
                                  (_right_bottom.get_x_cord() + _right_top.get_x_cord()) / 2, 
                                  (_right_bottom.get_y_cord() + _right_top.get_y_cord()) / 2
                                  );

        Point<T> geometric_center (
                                   (bottom_base_middle.get_x_cord() + top_base_middle.get_x_cord()) / 2, 
                                   (bottom_base_middle.get_y_cord() + top_base_middle.get_y_cord()) / 2
                                   );
        return geometric_center;
    }
}

template <class T>
T Trapeze<T>::calculate_perimeter() const noexcept {
    T left_side = sqrt(
                       (_left_top.get_x_cord() - _left_bottom.get_x_cord()) * 
                       (_left_top.get_x_cord() - _left_bottom.get_x_cord()) +                       
                       (_left_top.get_y_cord() - _left_bottom.get_y_cord()) * 
                       (_left_top.get_y_cord() - _left_bottom.get_y_cord())
                       );

    T right_side = sqrt(
                        (_right_top.get_x_cord() - _right_bottom.get_x_cord()) * 
                        (_right_top.get_x_cord() - _right_bottom.get_x_cord()) +
                        (_right_top.get_y_cord() - _right_bottom.get_y_cord()) * 
                        (_right_top.get_y_cord() - _right_bottom.get_y_cord())
                        );

    return (left_side + right_side + _bottom_base + _top_base);
}

template <class T>
T Trapeze<T>::calculate_area() const noexcept {
    return _height * (_bottom_base + _top_base) / 2;
}

template <class T>
const T& Trapeze<T>::get_bottom_base() const noexcept {
    return _bottom_base;
}

template <class T>
const T& Trapeze<T>::get_top_base() const noexcept {
    return _top_base;
}

template <class T>
const T& Trapeze<T>::get_height() const noexcept {
    return _height;
}

template <class T>
const T Trapeze<T>::get_middle_line() const noexcept {
    return (_bottom_base + _top_base) / 2;
}

template <class T>
Trapeze<T>::operator double() const {
    return _height * (_bottom_base + _top_base) / 2;
}

template <class T>
bool Trapeze<T>::operator==(const Trapeze<T>& other) const {
    return this->_left_bottom == other._left_bottom &&
           this->_right_bottom == other._right_bottom &&
           this->_left_top == other._left_top &&
           this->_right_top == other._right_top;
}

template <class T>
bool Trapeze<T>::operator!=(const Trapeze<T>& other) const {
    return !(*this == other);
}

template <class T>
std::pair<bool, bool> Trapeze<T>::check_trapeze(const Point<T>& first, const Point<T>& second, 
                                                const Point<T>& third, const Point<T>& fourth) 
                                                const noexcept {

    std::pair<bool, bool> res;

    Point<T> horizontal_bottom (
                                second.get_x_cord() - first.get_x_cord(), 
                                second.get_y_cord() - first.get_y_cord()
                                );

    Point<T> horizontal_top (
                             fourth.get_x_cord() - third.get_x_cord(), 
                             fourth.get_y_cord() - third.get_y_cord()
                             );

    T cos_between_bottom_top = (horizontal_bottom.get_x_cord() * horizontal_top.get_x_cord() + 
                                horizontal_bottom.get_y_cord() * horizontal_top.get_y_cord()) /
                               (sqrt(
                                     horizontal_bottom.get_x_cord() * 
                                     horizontal_bottom.get_x_cord() + 
                                     horizontal_bottom.get_y_cord() * 
                                     horizontal_bottom.get_y_cord()
                                    ) * 
                                sqrt(
                                     horizontal_top.get_x_cord() * 
                                     horizontal_top.get_x_cord() + 
                                     horizontal_top.get_y_cord() * 
                                     horizontal_top.get_y_cord()
                                     )
                                );

    Point<T> vertical_left (
                            third.get_x_cord() - first.get_x_cord(), 
                            third.get_y_cord() - first.get_y_cord()
                            );

    Point<T> vertical_right (
                             fourth.get_x_cord() - second.get_x_cord(), 
                             fourth.get_y_cord() - second.get_y_cord()
                             );

    T cos_between_left_right = (vertical_left.get_x_cord() * vertical_right.get_x_cord() + 
                                vertical_left.get_y_cord() * vertical_right.get_y_cord()) /
                               (sqrt(
                                     vertical_left.get_x_cord() * 
                                     vertical_left.get_x_cord() + 
                                     vertical_left.get_y_cord() * 
                                     vertical_left.get_y_cord()
                                     ) * 
                                sqrt(
                                     vertical_right.get_x_cord() * 
                                     vertical_right.get_x_cord() + 
                                     vertical_right.get_y_cord() * 
                                     vertical_right.get_y_cord()
                                     )
                                );

    if ((abs(cos_between_bottom_top - 1) < EPS && cos_between_left_right != 1) || 
        (cos_between_bottom_top != 1 && abs(cos_between_left_right - 1) < EPS)) {

        res.first = true;
    } else {
        res.first = false;
    }

    res.second = (abs(cos_between_bottom_top - 1) < EPS) ? true : false;
    return res;
}

namespace figures {

template <class U>
std::istream& operator>>(std::istream& is, Trapeze<U>& trapeze) {
    Point<U> left_bottom;
    Point<U> right_bottom;
    Point<U> left_top;
    Point<U> right_top;

    is >> left_bottom >> right_bottom >> left_top >> right_top;

    trapeze = Trapeze(left_bottom, right_bottom, left_top, right_top);
    return is; 
}

template <class U>
std::ostream& operator<<(std::ostream& os, const Trapeze<U>& trapeze) {
    os << "Left bottom point<T> coordinates: " << trapeze._left_bottom << std::endl;
    os << "Right bottom point<T> coordinates: " << trapeze._right_bottom << std::endl;
    os << "Left top point<T> coordinates: " << trapeze._left_top << std::endl;
    os << "Right top point<T> coordinates: " << trapeze._right_top << std::endl;
    os << "Bottom base of trapeze<T>: " << trapeze._bottom_base << std::endl;
    os << "Top base of trapeze<T>: " << trapeze._top_base << std::endl;
    os << "Height of trapeze<T>: " << trapeze._height << std::endl;

    return os;
}

}; // namespace figures
