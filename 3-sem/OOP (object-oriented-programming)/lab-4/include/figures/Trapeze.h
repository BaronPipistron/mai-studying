#pragma once

#include "../interfaces/figure.h"

namespace figures {

template <class T>
class Trapeze final : public Figure<T> {
private:
    Point<T> _left_bottom;
    Point<T> _right_bottom;
    Point<T> _left_top;
    Point<T> _right_top;

    T _bottom_base;
    T _top_base;
    T _height;

public:
    Trapeze() = default;
    Trapeze(const Point<T>&, const Point<T>&, const Point<T>&, const Point<T>&);
    Trapeze(const T&, const T&, const T&);
    Trapeze(const Trapeze<T>&) noexcept;
    Trapeze(Trapeze<T>&&) noexcept;

    Trapeze& operator=(const Trapeze<T>&) noexcept;
    Trapeze& operator=(Trapeze<T>&&) noexcept;

    virtual ~Trapeze() = default;

    virtual figures::Point<T> calculate_geometric_center() const noexcept override;
    virtual T calculate_perimeter() const noexcept override;
    virtual T calculate_area() const noexcept override;

    const T& get_bottom_base() const noexcept;
    const T& get_top_base() const noexcept;
    const T& get_height() const noexcept;
    const T get_middle_line() const noexcept;

    virtual operator double() const override;

    bool operator==(const Trapeze<T>&) const;
    bool operator!=(const Trapeze<T>&) const;

    template <class U>
    friend std::istream& operator>>(std::istream&, Trapeze<U>&);

    template <class U>
    friend std::ostream& operator<<(std::ostream&, const Trapeze<U>&);

private:
    std::pair<bool, bool> check_trapeze(
                                        const Point<T>&, 
                                        const Point<T>&, 
                                        const Point<T>&, 
                                        const Point<T>&
                                        ) const noexcept;
};

}; // namespace figures