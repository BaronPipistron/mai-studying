#include "../../include/functions/functions.h"

double functions::calculate_total_area(vector<std::shared_ptr<Figure<double>>>& vector) {
    double total_area { 0 };

    for (size_t i = 0; i != vector.size(); ++i) {
        total_area += (*vector[i]).calculate_area();
    }
    return total_area;
}

void functions::print_area(vector<std::shared_ptr<Figure<double>>>& vector) {
    std::cout << "Area of all figures in vector: " << std::endl;

    for (size_t i = 0; i != vector.size(); ++i) {
        std::cout << i << " " << (*vector[i]).calculate_area() << std::endl;
    }
}

void functions::print_geometric_center(vector<std::shared_ptr<Figure<double>>>& vector) {
    std::cout << "Geometric center of all figures in vector: " << std::endl;

    for (size_t i = 0; i != vector.size(); ++i) {
        std::cout << i << " " << (*vector[i]).calculate_geometric_center() << std::endl;
    }
}

void functions::print_perimeter(vector<std::shared_ptr<Figure<double>>>& vector) {
    std::cout << "Perimeter of all figures in vector: " << std::endl;

    for (size_t i = 0; i != vector.size(); ++i) {
        std::cout << i << " " << (*vector[i]).calculate_perimeter() << std::endl;
    }
}