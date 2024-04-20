#pragma once

#include "../interfaces/figure.hpp"

namespace figures {

class Trapeze final : public Figure {
private:
    Point _left_bottom;
    Point _right_bottom;
    Point _left_top;
    Point _right_top;

    double _bottom_base;
    double _top_base;
    double _height;

public:
    Trapeze() = default;
    Trapeze(const Point&, const Point&, const Point&, const Point&);
    Trapeze(double, double, double);
    Trapeze(const Trapeze&) noexcept;
    Trapeze(Trapeze&&) noexcept;

    Trapeze& operator=(const Trapeze&) noexcept;
    Trapeze& operator=(Trapeze&&) noexcept;

    virtual ~Trapeze() = default;

    virtual figures::Point calculate_geometric_center() const noexcept override;
    virtual double calculate_perimeter() const noexcept override;
    virtual double calculate_area() const noexcept override;

    const double get_bottom_base() const noexcept;
    const double get_top_base() const noexcept;
    const double get_height() const noexcept;
    const double get_middle_line() const noexcept;

    virtual operator double() const override;

    bool operator==(const Trapeze&) const;
    bool operator!=(const Trapeze&) const;

    friend std::istream& operator>>(std::istream&, Trapeze&);
    friend std::ostream& operator<<(std::ostream&, const Trapeze&);

private:
    std::pair<bool, bool> check_trapeze(const Point&, const Point&, const Point&, const Point&) const noexcept;
};

}; // namespace figures