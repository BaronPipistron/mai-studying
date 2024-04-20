#pragma once

#include "../interfaces/figure.hpp"

namespace figures {

class Square final : public Figure {
private:
    Point _left_bottom;
    Point _right_bottom;
    Point _left_top;
    Point _right_top;

    double _side;

public:
    Square() = default;
    Square(const Point&, const Point&, const Point&, const Point&);
    Square(const double);
    Square(const Square&) noexcept;
    Square(Square&&) noexcept;

    Square& operator=(const Square&) noexcept;
    Square& operator=(Square&&) noexcept;

    virtual ~Square() = default;

    virtual figures::Point calculate_geometric_center() const noexcept override;
    virtual double calculate_perimeter() const noexcept override;
    virtual double calculate_area() const noexcept override;

    const double get_side() const noexcept;

    virtual operator double() const override;

    bool operator==(const Square&) const;
    bool operator!=(const Square&) const;

    friend std::istream& operator>>(std::istream&, Square&);
    friend std::ostream& operator<<(std::ostream&, const Square&);

private:
    bool check_square(const Point&, const Point&, const Point&, const Point&) const noexcept;
};

}; // namespace figures