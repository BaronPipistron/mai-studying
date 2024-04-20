#pragma once

#include "../interfaces/figure.hpp"
#include <iostream>

namespace figures {

class Rectangle final : public Figure {
private:
    Point _left_bottom;
    Point _right_bottom;
    Point _left_top;
    Point _right_top;

    double _length;
    double _width;

public:
    Rectangle() = default;
    Rectangle(const Point&, const Point&, const Point&, const Point&);
    Rectangle(const double, const double);
    Rectangle(const Rectangle&) noexcept;
    Rectangle(Rectangle&&) noexcept;

    Rectangle& operator=(const Rectangle&) noexcept;
    Rectangle& operator=(Rectangle&&) noexcept;

    virtual ~Rectangle() = default;

    virtual figures::Point calculate_geometric_center() const noexcept override;
    virtual double calculate_perimeter() const noexcept override;
    virtual double calculate_area() const noexcept override;

    const double get_length() const noexcept;
    const double get_width() const noexcept;

    virtual operator double() const override;

    bool operator==(const Rectangle&) const;
    bool operator!=(const Rectangle&) const;

    friend std::istream& operator>>(std::istream&, Rectangle&);
    friend std::ostream& operator<<(std::ostream&, const Rectangle&);

private:
    bool check_rectangle(const Point&, const Point&, const Point&, const Point&) const noexcept;
};

}; // namespace figures