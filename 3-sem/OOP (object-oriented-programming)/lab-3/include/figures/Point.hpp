#pragma once

#include <iostream>

namespace figures {

class Point final {
private:
    double _x;
    double _y;

public:
    Point() = default;
    Point(double, double) noexcept;
    Point(const std::initializer_list<double>&);
    Point(const Point&) noexcept;
    Point(Point&&) noexcept;

    Point& operator=(const Point&) noexcept;
    Point& operator=(Point&&) noexcept;

    const double get_x_cord() const noexcept;
    const double get_y_cord() const noexcept;

    void set_x_cord(double) noexcept;
    void set_y_cord(double) noexcept;

    bool operator==(const Point& other) const noexcept;
    bool operator!=(const Point& other) const noexcept;

    friend std::istream& operator>>(std::istream&, Point&);
    friend std::ostream& operator<<(std::ostream&, const Point&);

    virtual ~Point() = default;
};

}; // namespace figures