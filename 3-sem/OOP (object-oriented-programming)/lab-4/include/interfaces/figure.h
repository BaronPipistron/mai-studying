#pragma once

#include <cmath>
#include <iostream>
#include "../figures/Point.h"

namespace figures {

template <class T>
class Figure {
protected:
    Figure() = default;

public:
    virtual ~Figure() = default;

    virtual figures::Point<T> calculate_geometric_center() const = 0;
    virtual double calculate_perimeter() const = 0;
    virtual double calculate_area() const = 0;

    virtual operator double() const = 0;
};

}; // namespace figures