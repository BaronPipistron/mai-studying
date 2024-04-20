#ifndef MATRIXHPP
#define MATRIXHPP

#include "MyVector.h"
#include <iostream>

class Matrix{
private:
    myVector<int> line_matrix;
    size_t matrix_lines;
    size_t matrix_columns;

public:
    Matrix();
    Matrix(size_t lines, size_t columns);

    friend std::istream& operator>>(std::istream& is, Matrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, Matrix& matrix);

    myVector<int> multiply(const myVector<int>& vector_string);
    void print();
};

#include "../src/Matrix.cpp"

#endif 