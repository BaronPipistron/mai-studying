#include "../include/Twelve.hpp"

#define BASE 12

using namespace MAI::OOP::Lab2;

Twelve::Twelve(const size_t size, unsigned char val) noexcept {
    _duodecimal_number.resize(size, val);
}

Twelve::Twelve(const std::initializer_list<unsigned char>& list) {
    for (unsigned char c: list) {
        if ((c >= '0' && c <= '9') || c == 'A' || c == 'B') {
            _duodecimal_number.push_back(c);
        } else {
            throw std::invalid_argument("List contains not duodecimal digit");
        }
    }

    for (size_t left = 0, right = _duodecimal_number.size() - 1; left < right; ++left, --right) {
        unsigned char tmp = _duodecimal_number[left];
        _duodecimal_number[left] = _duodecimal_number[right];
        _duodecimal_number[right] = tmp;
    }
}

Twelve::Twelve(const std::string& str) {
    for (int64_t i = str.length() - 1; i >= 0; --i) {
        if ((str[i] >= '0' && str[i] <= '9') || str[i] == 'A' || str[i] == 'B') {
            _duodecimal_number.push_back(str[i]);
        } else {
            throw std::invalid_argument("String contains not duodecimal digit");
        }
    }
}

Twelve::Twelve(const Twelve& other) noexcept {
    this->_duodecimal_number = other._duodecimal_number;
}

Twelve::Twelve(Twelve&& other) noexcept {
    this->_duodecimal_number = std::move(other._duodecimal_number);
}

Twelve& Twelve::operator=(const Twelve& other) noexcept {
    if (this == &other) return *this;

    this->_duodecimal_number = other._duodecimal_number;
    return *this;
}

Twelve& Twelve::operator=(Twelve&& other) noexcept {
    this->_duodecimal_number = std::move(other._duodecimal_number);
    return *this;
}

size_t Twelve::bit_depth_of_num() const noexcept {
    return _duodecimal_number.size();
}

void Twelve::remove_leading_zeros() noexcept {
    if (_duodecimal_number.size() == 0) return;
 
    while (_duodecimal_number.back() == '0') {
        if (_duodecimal_number.size() == 1) break;
        _duodecimal_number.pop_back();
    }
}

uint64_t Twelve::to_DEC() const noexcept {
    uint64_t DEC_num { 0 };

    for (size_t i = 0; i != _duodecimal_number.size(); ++i) {
        DEC_num +=  digit_to_int(_duodecimal_number[i]) * static_cast<uint64_t>(pow(BASE, i));
    }

    return DEC_num;
}

std::string Twelve::to_BIN() const noexcept {
    std::string BIN_num { "" };
    uint64_t DEC_num { to_DEC() };

    while (DEC_num > 0) {
        BIN_num.insert(BIN_num.begin(), '0' + (DEC_num & 1));
        DEC_num >>= 1;
    }

    return BIN_num;
}

Twelve& Twelve::operator+=(const Twelve& other) noexcept {
    size_t max_size = std::max(this->_duodecimal_number.size(), other._duodecimal_number.size());
    size_t min_size = std::min(this->_duodecimal_number.size(), other._duodecimal_number.size());

    uint64_t carry { 0 };
    for (size_t i = 0; i != min_size; ++i) {
        uint64_t sum = carry + digit_to_int(this->_duodecimal_number[i]) + digit_to_int(other._duodecimal_number[i]);
        if (sum >= BASE) {
            this->_duodecimal_number[i] = digit_to_char(sum % BASE);
            carry = 1;
        } else {
            this->_duodecimal_number[i] = digit_to_char(sum);
            carry = 0;
        }
    }

    if (max_size == min_size) {
        if (carry == 1) this->_duodecimal_number.push_back('1');
        return *this;
    }

    if (max_size ==  this->_duodecimal_number.size()){
        for (size_t i = min_size; i != max_size || carry != 0; ++i) {
            if (i > max_size) break;
            uint64_t sum = carry + digit_to_int(this->_duodecimal_number[i]);
            if (sum >= BASE) {
                this->_duodecimal_number[i] = digit_to_char(sum % BASE);
                carry = 1;
            } else {
                this->_duodecimal_number[i] = digit_to_char(sum);
                carry = 0;
            }
        }
    } else {
        this->_duodecimal_number.resize(max_size, '0');

        for (size_t i = min_size; i != max_size || carry != 0; ++i) {
            if (i > max_size) break;
            uint64_t sum = carry + digit_to_int(other._duodecimal_number[i]);
            if (sum >= BASE) {
                this->_duodecimal_number[i] = digit_to_char(sum % BASE);
                carry = 1;
            } else {
                this->_duodecimal_number[i] = digit_to_char(sum);
                carry = 0;
            }
        }
    }
    
    if (carry == 1) this->_duodecimal_number.push_back('1');
    remove_leading_zeros();
    return *this;
}

Twelve& Twelve::operator-=(const Twelve& other) {
    if (*this < other) {
        throw std::invalid_argument("The subtracted is greater than the reduced");
    }

    uint64_t carry { 0 };
    for (size_t i = 0; i != other._duodecimal_number.size(); ++i) {
        uint64_t difference { 0 };
        if (this->_duodecimal_number[i] < other._duodecimal_number[i]) {
            difference = BASE + digit_to_int(this->_duodecimal_number[i]) - carry - digit_to_int(other._duodecimal_number[i]);
            carry = 1;   
        } else {
            uint64_t digit = digit_to_int(this->_duodecimal_number[i]) - carry;
            if (digit < digit_to_int(other._duodecimal_number[i])){
                difference = BASE + digit - digit_to_int(other._duodecimal_number[i]);
                carry = 1;
            } else{
                difference = digit_to_int(this->_duodecimal_number[i]) - carry - digit_to_int(other._duodecimal_number[i]);
                carry = 0;
            }
        }
        this->_duodecimal_number[i] = digit_to_char(difference);
    }

    for (size_t i = other._duodecimal_number.size(); i != this->_duodecimal_number.size() || carry != 0; ++i) {
        int64_t difference = digit_to_int(this->_duodecimal_number[i]) - carry;
        if (difference < 0) {
            difference = BASE + digit_to_int(this->_duodecimal_number[i]) - carry;
            carry = 1;
        } else {
            carry = 0;
        }
        this->_duodecimal_number[i] = digit_to_char(difference);
    }

    this->remove_leading_zeros();
    return *this;
}

Twelve& Twelve::operator++() noexcept {
    for (size_t i = 0; i != _duodecimal_number.size(); ++i) {
        if (_duodecimal_number[i] >= '0' && _duodecimal_number[i] < '9') {
            ++_duodecimal_number[i];
            return *this;
        } else if (_duodecimal_number[i] == '9') {
            _duodecimal_number[i] = 'A';
            return *this;
        } else if (_duodecimal_number[i] == 'A') {
            ++_duodecimal_number[i];
            return *this;
        } else {
            _duodecimal_number[i] = '0';
        }
    }

    _duodecimal_number.push_back('1');
    return *this;
}

Twelve& Twelve::operator--() {
    if (_duodecimal_number.size() == 1 && _duodecimal_number[0] == '0') {
        throw std::runtime_error("Result of decrement less than zero");
    }

    for (size_t i = 0; i != _duodecimal_number.size(); ++i) {
        if (_duodecimal_number[i] > '0' && _duodecimal_number[i] <= '9') {
            --_duodecimal_number[i];
            remove_leading_zeros();
            return *this;
        } else if (_duodecimal_number[i] == 'A') {
            _duodecimal_number[i] = '9';
            remove_leading_zeros();
            return *this;
        } else if (_duodecimal_number[i] == 'B') {
            --_duodecimal_number[i];
            remove_leading_zeros();
            return *this;
        } else {
            _duodecimal_number[i] = 'B';
        }
    }

    remove_leading_zeros();
    return *this;
}

Twelve Twelve::operator++(int) noexcept {
    Twelve tmp = *this;
    ++(*this);
    return tmp;
}

Twelve Twelve::operator--(int) {
    Twelve tmp = *this;
    --(*this);
    return tmp;
}

Twelve Twelve::operator+(const Twelve& other) const noexcept {
    Twelve sum = *this;
    return sum += other;
}

Twelve Twelve::operator-(const Twelve& other) const {
    if (*this < other) {
        throw std::invalid_argument("The subtracted is greater than the reduced");
    }

    Twelve difference = *this;
    return difference -= other;
}

bool Twelve::operator==(const Twelve& other) const noexcept {
    return (this->_duodecimal_number == other._duodecimal_number);
}

bool Twelve::operator!=(const Twelve& other) const noexcept {
    return (this->_duodecimal_number != other._duodecimal_number);
}

bool Twelve::operator>(const Twelve& other) const noexcept {
    if (this->bit_depth_of_num() > other.bit_depth_of_num()) return true;
    if (this->bit_depth_of_num() < other.bit_depth_of_num()) return false;

    for (int64_t i = this->bit_depth_of_num() - 1; i >= 0; --i) {
        if (this->_duodecimal_number[i] > other._duodecimal_number[i]) return true; 
    }

    return false; 
}

bool Twelve::operator<(const Twelve& other) const noexcept {
    if (this->bit_depth_of_num() < other.bit_depth_of_num()) return true;
    if (this->bit_depth_of_num() > other.bit_depth_of_num()) return false;

    for (int64_t i = this->bit_depth_of_num() - 1; i >= 0; --i) {
        if (this->_duodecimal_number[i] < other._duodecimal_number[i]) return true; 
    }

    return false; 
}

bool Twelve::operator>=(const Twelve& other) const noexcept {
    return (*this > other || *this == other);
}

bool Twelve::operator<=(const Twelve& other) const noexcept {
    return (*this < other || *this == other);
}

void Twelve::print() const noexcept {
    if (_duodecimal_number.size() == 0) {
        std::cout << '0' << std::endl;
    }

    for (int64_t i = _duodecimal_number.size() - 1; i >= 0; --i) {
        std::cout << _duodecimal_number[i];
    }
    std::cout << std::endl;
}

unsigned char Twelve::digit_to_char(uint64_t num) const noexcept {
    if (num >= 0 && num <= 9) {
        return num + '0';
    } else {
        return num - 10 + 'A';
    }
}

uint64_t Twelve::digit_to_int(unsigned char c) const noexcept {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else {
        return c - 'A' + 10;
    }
}

namespace MAI::OOP::Lab2 {

std::ostream& operator<<(std::ostream& os, const Twelve& num) {
    if (num._duodecimal_number.size() == 0) {
        os << '0';
        return os;
    }

    for (int64_t i = num._duodecimal_number.size() - 1; i >= 0; --i) {
        os << num._duodecimal_number[i];
    }

    return os;
}

}; // namespace MAI::OOP::Lab2