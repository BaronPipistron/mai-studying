#pragma once

#include "vector.hpp"
#include "../src/vector.cpp"

#include <cmath>
#include <iostream>
#include <string>

namespace MAI::OOP::Lab2 {

class Twelve final {
private:
    vector<unsigned char> _duodecimal_number;

public:
    Twelve() = default;
    Twelve(const size_t, unsigned char = 0) noexcept;
    Twelve(const std::initializer_list<unsigned char>&);
    Twelve(const std::string&);
    Twelve(const Twelve&) noexcept;
    Twelve(Twelve&&) noexcept;
    virtual ~Twelve() noexcept = default;

    Twelve& operator=(const Twelve&) noexcept;
    Twelve& operator=(Twelve&&) noexcept;

    size_t bit_depth_of_num() const noexcept;
    void remove_leading_zeros() noexcept;
    uint64_t to_DEC() const noexcept;
    std::string to_BIN() const noexcept;

    Twelve& operator+=(const Twelve&) noexcept;
    Twelve& operator-=(const Twelve&);
    Twelve& operator++() noexcept;
    Twelve& operator--();
    Twelve operator++(int) noexcept;
    Twelve operator--(int);
    Twelve operator+(const Twelve&) const noexcept;
    Twelve operator-(const Twelve&) const;

    bool operator==(const Twelve&) const noexcept;
    bool operator!=(const Twelve&) const noexcept;
    bool operator>(const Twelve&) const noexcept;
    bool operator<(const Twelve&) const noexcept;
    bool operator>=(const Twelve&) const noexcept;
    bool operator<=(const Twelve&) const noexcept;

    void print() const noexcept;
    friend std::ostream& operator<<(std::ostream&, const Twelve&);

private:
    unsigned char digit_to_char(uint64_t) const noexcept;
    uint64_t digit_to_int(unsigned char) const noexcept;
};

}; // namespace MAI::OOP::Lab2
