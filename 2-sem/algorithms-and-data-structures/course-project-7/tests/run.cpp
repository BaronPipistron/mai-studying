#include <iostream>

#include "../include/MyVector.h"
#include "../include/Matrix.h"
#include "../src/benchmark.cpp"

int main(){

    myVector<int> vector_string = {4, 10, 52, 0, 34, 47, -4, 9};
    Matrix sparse_matrix;

    std::cin >> sparse_matrix;
    myVector<int> res = sparse_matrix.multiply(vector_string);

    benchmark();
    return 0;
}