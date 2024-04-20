#include <iostream>
#include <memory>

#include "include/containers/vector.h"
#include "src/containers/vector.cpp"
#include "include/functions/functions.h"
#include "src/functions/functions.cpp"

#include "include/figures/Rectangle.h"
#include "src/figures/Rectangle.cpp"
#include "include/figures/Square.h"
#include "src/figures/Square.cpp"
#include "include/figures/Trapeze.h"
#include "src/figures/Trapeze.cpp"
#include "include/figures/Point.h"
#include "src/figures/Point.cpp"

using namespace figures;
using namespace functions;

using rectangle = Rectangle<double>;
using square = Square<double>;
using trapeze = Trapeze<double>;

int main() {
    containers::vector<std::shared_ptr<Figure<double>>> figures_vector;

    std::shared_ptr<rectangle> rect = std::make_shared<rectangle>();
    std::cout << "Input rectangle coordinates" << std::endl;
    std::cin >> *rect;
    std::cout << *rect << std::endl;
    figures_vector.push_back(rect);

    std::shared_ptr<rectangle> rectangle_ptr = std::make_shared<rectangle>(5, 3);
    std::shared_ptr<square> square_ptr = std::make_shared<square>(10);
    std::shared_ptr<trapeze> trapeze_ptr = std::make_shared<trapeze>(1, 3, 5);   

    figures_vector.push_back(rectangle_ptr);
    figures_vector.push_back(square_ptr);
    figures_vector.push_back(trapeze_ptr);

    print_geometric_center(figures_vector);
    print_area(figures_vector);
    print_perimeter(figures_vector);

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << calculate_total_area(figures_vector) << std::endl;

    std::shared_ptr<rectangle> rectangle_ptr1 = std::make_shared<rectangle>(5, 5);
    std::shared_ptr<rectangle> rectangle_ptr2 = std::make_shared<rectangle>(4, 1);
    std::shared_ptr<rectangle> rectangle_ptr3 = std::make_shared<rectangle>(8, 90);

    std::shared_ptr<square> square_ptr1 = std::make_shared<square>(4);
    std::shared_ptr<square> square_ptr2 = std::make_shared<square>(8);
    std::shared_ptr<square> square_ptr3 = std::make_shared<square>(3);

    std::shared_ptr<trapeze> trapeze_ptr1 = std::make_shared<trapeze>(2, 4, 7);
    std::shared_ptr<trapeze> trapeze_ptr2 = std::make_shared<trapeze>(3, 5, 1);
    std::shared_ptr<trapeze> trapeze_ptr3 = std::make_shared<trapeze>(8, 1, 1);

    figures_vector.push_back(rectangle_ptr1);
    figures_vector.push_back(rectangle_ptr2);
    figures_vector.push_back(rectangle_ptr3);
    figures_vector.push_back(square_ptr1);
    figures_vector.push_back(square_ptr2);
    figures_vector.push_back(square_ptr3);
    figures_vector.push_back(trapeze_ptr1);
    figures_vector.push_back(trapeze_ptr2);
    figures_vector.push_back(trapeze_ptr3);

    print_geometric_center(figures_vector);
    print_area(figures_vector);
    print_perimeter(figures_vector);

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << calculate_total_area(figures_vector) << std::endl;

    figures_vector.erase(3);
    figures_vector.erase(7);

    print_geometric_center(figures_vector);
    print_area(figures_vector);
    print_perimeter(figures_vector);

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << calculate_total_area(figures_vector) << std::endl;

    figures_vector.pop_back();
    figures_vector.pop_back();

    print_geometric_center(figures_vector);
    print_area(figures_vector);
    print_perimeter(figures_vector);

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << calculate_total_area(figures_vector) << std::endl;

    return 0;
}