#pragma once

#include "../interfaces/figure.h"

namespace figures {

template <class T>
class Square final : public Figure<T> {
private:
    Point<T> _left_bottom;
    Point<T> _right_bottom;
    Point<T> _left_top;
    Point<T> _right_top;

    T _side;

public:
    Square() = default;
    Square(const Point<T>&, const Point<T>&, const Point<T>&, const Point<T>&);
    Square(const T&);
    Square(const Square<T>&) noexcept;
    Square(Square<T>&&) noexcept;

    Square<T>& operator=(const Square<T>&) noexcept;
    Square<T>& operator=(Square<T>&&) noexcept;

    virtual ~Square() = default;

    virtual figures::Point<T> calculate_geometric_center() const noexcept override;
    virtual T calculate_perimeter() const noexcept override;
    virtual T calculate_area() const noexcept override;

    const T& get_side() const noexcept;

    virtual operator double() const override;

    bool operator==(const Square<T>&) const;
    bool operator!=(const Square<T>&) const;

    template <class U>
    friend std::istream& operator>>(std::istream&, Square<U>&);

    template <class U>
    friend std::ostream& operator<<(std::ostream&, const Square<U>&);

private:
    bool check_square(
                      const Point<T>&, 
                      const Point<T>&, 
                      const Point<T>&, 
                      const Point<T>&
                      ) const noexcept;
};

}; // namespace figures