#include "../../include/figures/Square.hpp"

using namespace figures;

Square::Square(const Point& left_bottom, const Point& right_bottom,
               const Point& left_top, const Point& right_top) {

    if (check_square(left_bottom, right_bottom, left_top, right_top) == false) {
        throw std::invalid_argument("Given points do not form a square");
    }

    this->_left_bottom = left_bottom;
    this->_right_bottom = right_bottom;
    this->_left_top = left_top;
    this->_right_top = right_top;

    this->_side = sqrt((_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) * (_right_bottom.get_x_cord() - _left_bottom.get_x_cord()) +
                       (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()) * (_right_bottom.get_y_cord() - _left_bottom.get_y_cord()));
}

Square::Square(const double side) {
    if (side <= 0) {
        throw std::invalid_argument("Side of square must be more than zero");
    }

    this->_left_bottom = Point(0, 0);
    this->_right_bottom = Point(side, 0);
    this->_left_top = Point(0, side);
    this->_right_top = Point(side, side);

    this->_side = side;
}

Square::Square(const Square& other) noexcept
              : _left_bottom(other._left_bottom), _right_bottom(other._right_bottom),
                _left_top(other._left_top), _right_top(other._right_top),
                _side(other._side) {}

Square::Square(Square&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_side = std::move(other._side);
}

Square& Square::operator=(const Square& other) noexcept {
    if (this == &other) return *this;

    this->_left_bottom = other._left_bottom;
    this->_right_bottom = other._right_bottom;
    this->_left_top = other._left_top;
    this->_right_top = other._right_top;

    this->_side = other._side;
    return *this;
}

Square& Square::operator=(Square&& other) noexcept {
    this->_left_bottom = std::move(other._left_bottom);
    this->_right_bottom = std::move(other._right_bottom);
    this->_left_top = std::move(other._left_top);
    this->_right_top = std::move(other._right_top);

    this->_side = std::move(other._side);
    return *this;
}

figures::Point Square::calculate_geometric_center() const noexcept {
    Point geometric_center ((_right_top.get_x_cord() + _left_bottom.get_x_cord()) / 2,
                            (_right_top.get_y_cord() + _left_bottom.get_y_cord()) / 2);
    return geometric_center;
}

double Square::calculate_perimeter() const noexcept {
    return (_side * 4);
}

double Square::calculate_area() const noexcept {
    return (_side * _side);
}

const double Square::get_side() const noexcept {
    return _side;
}

Square::operator double() const {
    return (_side * _side);
}

bool Square::operator==(const Square& other) const {
    return this->_left_bottom == other._left_bottom &&
           this->_right_bottom == other._right_bottom &&
           this->_left_top == other._left_top &&
           this->_right_top == other._right_top;
}

bool Square::operator!=(const Square& other) const {
    return !(*this == other);
}

bool Square::check_square(const Point& first, const Point& second, 
                          const Point& third, const Point& fourth) const noexcept {

    double c_x = (first.get_x_cord() + second.get_x_cord() + third.get_x_cord() + fourth.get_x_cord()) / 4;
    double c_y = (first.get_y_cord() + second.get_y_cord() + third.get_y_cord() + fourth.get_y_cord()) / 4;

    double dd1 = (c_x - first.get_x_cord()) * (c_x - first.get_x_cord()) + (c_y - first.get_y_cord()) * (c_y - first.get_y_cord());
    double dd2 = (c_x - second.get_x_cord()) * (c_x - second.get_x_cord()) + (c_y - second.get_y_cord()) * (c_y - second.get_y_cord());
    double dd3 = (c_x - third.get_x_cord()) * (c_x - third.get_x_cord()) + (c_y - third.get_y_cord()) * (c_y - third.get_y_cord());
    double dd4 = (c_x - fourth.get_x_cord()) * (c_x - fourth.get_x_cord()) + (c_y - fourth.get_y_cord()) * (c_y - fourth.get_y_cord());

    double length = sqrt((second.get_x_cord() - first.get_x_cord()) * (second.get_x_cord() - first.get_x_cord()) +
                         (second.get_y_cord() - first.get_y_cord()) * (second.get_y_cord() - first.get_y_cord()));

    double width = sqrt((third.get_x_cord() - first.get_x_cord()) * (third.get_x_cord() - first.get_x_cord()) +
                        (third.get_y_cord() - first.get_y_cord()) * (third.get_y_cord() - first.get_y_cord()));

    bool compare_sides = (length == width);

    return (dd1 == dd2 && dd1 == dd3 && dd1 == dd4) && compare_sides;
}

namespace figures {

std::istream& operator>>(std::istream& is, Square& square) {
    is >> square._left_bottom >> square._right_bottom >> square._left_top >> square._right_top;
    return is; 
}

std::ostream& operator<<(std::ostream& os, const Square& square) {
    os << "Left bottom point coordinates: " << square._left_bottom << std::endl;
    os << "Right bottom point coordinates: " << square._right_bottom << std::endl;
    os << "Left top point coordinates: " << square._left_top << std::endl;
    os << "Right top point coordinates: " << square._right_top << std::endl;
    os << "Side of square: " << square._side << std::endl; 
    return os;
}

}; // namespace figures