#include "../include/Matrix.h"

#include <iostream>

Matrix::Matrix(): matrix_lines(0), matrix_columns(0){
    line_matrix.push_back(0);
    line_matrix.push_back(0);
}

Matrix::Matrix(size_t lines, size_t columns): matrix_lines(lines), matrix_columns(columns){
    line_matrix.push_back(0);
    line_matrix.push_back(0);
}

std::istream& operator>>(std::istream& is, Matrix& matrix){
    matrix.line_matrix.pop_back();
    size_t line, columns;
    int input_value;

    is >> line >> columns;

    matrix.matrix_lines = line;
    matrix.matrix_columns = columns;

    for (size_t i = 0; i != line; ++i){
        for (size_t j = 0; j != columns; ++j){
            is >> input_value;
            if (input_value != 0){
                if (matrix.line_matrix.back() == 0){
                    matrix.line_matrix.push_back(i + 1);
                }
                matrix.line_matrix.push_back(j + 1);
                matrix.line_matrix.push_back(input_value);
            }
        }

        if (matrix.line_matrix.back() != 0){
            matrix.line_matrix.push_back(0);
        }
    }
    matrix.line_matrix.push_back(0);

    return is;
}

std::ostream& operator<<(std::ostream& os, Matrix& matrix){
    for (size_t i = 0; i != matrix.line_matrix.size(); ++i){
        os << matrix.line_matrix[i] << " ";
    }

    return os;
}

myVector<int> Matrix::multiply(const myVector<int>& vector_string){
    if (vector_string.size() != matrix_lines){
        throw std::range_error("Vector columns not equal matrix lines");
    }

    myVector<int> result;

    for (size_t i = 0; i != matrix_columns; ++i){
        result.push_back(0);
    }

    size_t cur_line = 0;

    while (cur_line < line_matrix.size()){
        int j = line_matrix[cur_line];
        if (j == 0){
            ++cur_line;
            if (cur_line >= line_matrix.size() || line_matrix[cur_line] == 0) break;
            j = line_matrix[cur_line];
        }

        int cur_column = cur_line + 1;
        int column_of_result_vector = line_matrix[cur_line] - 1;

        for (size_t column = cur_column; line_matrix[column] != 0; column += 2){
            int value = line_matrix[column + 1];
            result[line_matrix[column] - 1] += (vector_string[column_of_result_vector] * value);
            cur_line = column;
        }
        
        ++cur_line;
    }

    uint64_t number_of_non_zero_elements = 0;
    std::cout << "Result: ( ";
    for (size_t i = 0; i != result.size(); ++i){
        if (result[i] != 0) ++number_of_non_zero_elements;
        std::cout << result[i] << " ";
    }
    std::cout << ")\nNumber of non-zeros elements of the result: " << number_of_non_zero_elements << "\n";

    return result;
}

void Matrix::print(){
    size_t line = 1, column = 1;
    for (size_t i = 0; i < line_matrix.size(); ++i) {

        if (!line_matrix[i]) {
            ++i;
            if (!line_matrix[i]) {
                while (line <= matrix_lines) {
                    while (column <= matrix_columns) {
                        column++;
                        std::cout << 0 << " ";
                    }
                    std::cout << '\n';
                    line++;
                    column = 1;
                }
                return;
            }
            while (line < line_matrix[i]) {
                while (column <= matrix_columns) {
                    column++;
                    std::cout << 0 << " ";
                }
                std::cout << '\n';
                line++;
                column = 1;
            }
        }
        else {
            while (column != line_matrix[i]) {
                column++;
                std::cout << 0 << " ";
            }
            ++i;
            column++;
            std::cout << line_matrix[i] << " ";
        }
    }
}