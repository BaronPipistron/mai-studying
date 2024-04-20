#include "../../include/containers/FiguresVector.hpp"
#include "vector.cpp"

using namespace figures;

FiguresVector::FiguresVector(const FiguresVector& other) {
    this->_vec = other._vec;
}

FiguresVector::FiguresVector(FiguresVector&& other) noexcept {
    this->_vec = std::move(other._vec);
}

FiguresVector& FiguresVector::operator=(const FiguresVector& other) {
    this->_vec = other._vec;
    return *this;
}

FiguresVector& FiguresVector::operator=(FiguresVector&& other) noexcept {
    this->_vec = std::move(other._vec);
    return *this;
}

void FiguresVector::print_geometric_center() const noexcept {
    std::cout << std::endl << "Geometric center" << std::endl;
    for (size_t i = 0; i != _vec.size(); ++i) {
        std::cout << i << " " << (*_vec[i]).calculate_geometric_center() << std::endl;
    }
}

void FiguresVector::print_area() const noexcept {
    std::cout << std::endl << "Area" << std::endl;
    for (size_t i = 0; i != _vec.size(); ++i) {
        std::cout << i << " " << (*_vec[i]).calculate_area() << std::endl;
    }
}

void FiguresVector::print_perimeter() const noexcept {
    std::cout << std::endl << "Perimeter" << std::endl;
    for (size_t i = 0; i != _vec.size(); ++i) {
        std::cout << i << " " << (*_vec[i]).calculate_perimeter() << std::endl;
    }
}

double FiguresVector::calculate_total_area() const noexcept {
    double total_area { 0 };
    for (size_t i = 0; i != _vec.size(); ++i) {
        total_area += (*_vec[i]).calculate_area();
    }

    return total_area;
}

void FiguresVector::push_back(Figure* figure) {
    _vec.push_back(figure);
}

void FiguresVector::pop_back() {
    _vec.pop_back();
}

void FiguresVector::erase(size_t index) {
    _vec.erase(index);
}

Figure* FiguresVector::operator[](size_t index) noexcept {
    return _vec[index];
}

Figure* FiguresVector::front() noexcept {
    return _vec.front();
}

Figure* FiguresVector::back() noexcept {
    return _vec.back();
}

const Figure* FiguresVector::operator[](size_t index) const noexcept {
    return _vec[index];
}

const Figure* FiguresVector::front() const noexcept {
    return _vec.front();
}

const Figure* FiguresVector::back() const noexcept {
    return _vec.back();
}

size_t FiguresVector::size() const noexcept {
    return _vec.size();
}

size_t FiguresVector::capacity() const noexcept {
    return _vec.capacity();
}

bool FiguresVector::empty() const noexcept {
    return _vec.empty();
}