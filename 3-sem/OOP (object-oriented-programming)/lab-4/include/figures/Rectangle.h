#pragma once

#include "../interfaces/figure.h"
#include <iostream>

namespace figures {

template <class T>
class Rectangle final : public Figure<T> {
private:
    Point<T> _left_bottom;
    Point<T> _right_bottom;
    Point<T> _left_top;
    Point<T> _right_top;

    T _length;
    T _width;

public:
    Rectangle() = default;
    Rectangle(const Point<T>&, const Point<T>&, const Point<T>&, const Point<T>&);
    Rectangle(const T&, const T&);
    Rectangle(const Rectangle<T>&) noexcept;
    Rectangle(Rectangle<T>&&) noexcept;

    Rectangle<T>& operator=(const Rectangle<T>&) noexcept;
    Rectangle<T>& operator=(Rectangle<T>&&) noexcept;

    virtual ~Rectangle() = default;

    virtual figures::Point<T> calculate_geometric_center() const noexcept override;
    virtual T calculate_perimeter() const noexcept override;
    virtual T calculate_area() const noexcept override;

    const T& get_length() const noexcept;
    const T& get_width() const noexcept;

    virtual operator double() const override;

    bool operator==(const Rectangle<T>&) const;
    bool operator!=(const Rectangle<T>&) const;

    template <class U>
    friend std::istream& operator>>(std::istream&, Rectangle<U>&);

    template <class U>
    friend std::ostream& operator<<(std::ostream&, const Rectangle<U>&);

private:
    bool check_rectangle(
                         const Point<T>&, 
                         const Point<T>&, 
                         const Point<T>&, 
                         const Point<T>&
                         ) const noexcept;
};

}; // namespace figures