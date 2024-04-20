#include <iostream>

#include "include/containers/vector.hpp"
#include "src/containers/vector.cpp"
#include "include/containers/FiguresVector.hpp"

#include "include/figures/Rectangle.hpp"
#include "include/figures/Square.hpp"
#include "include/figures/Trapeze.hpp"

using namespace figures;

int main() {
    FiguresVector figures_vector;

    Rectangle* rect = new Rectangle;
    std::cout << "Input rectangle coordinates" << std::endl;
    std::cin >> *rect;
    std::cout << *rect << std::endl;
    figures_vector.push_back(static_cast<Figure*>(rect));

    Rectangle* rectangle_ptr = new Rectangle(5, 3);
    Square* square_ptr = new Square(10);
    Trapeze* trapeze_ptr = new Trapeze(1, 3, 5);

    figures_vector.push_back(static_cast<Figure*>(rectangle_ptr));
    figures_vector.push_back(static_cast<Figure*>(square_ptr));
    figures_vector.push_back(static_cast<Figure*>(trapeze_ptr));

    figures_vector.print_geometric_center();
    figures_vector.print_area();
    figures_vector.print_perimeter();

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << figures_vector.calculate_total_area() << std::endl;

    Rectangle* rectangle_ptr1 = new Rectangle(5, 5);
    Rectangle* rectangle_ptr2 = new Rectangle(4, 1);
    Rectangle* rectangle_ptr3 = new Rectangle(8, 90);

    Square* square_ptr1 = new Square(4);
    Square* square_ptr2 = new Square(8);
    Square* square_ptr3 = new Square(3);

    Trapeze* trapeze_ptr1 = new Trapeze(2, 4, 7);
    Trapeze* trapeze_ptr2 = new Trapeze(3, 5, 1);
    Trapeze* trapeze_ptr3 = new Trapeze(8, 1, 1);

    figures_vector.push_back(static_cast<Figure*>(rectangle_ptr1));
    figures_vector.push_back(static_cast<Figure*>(rectangle_ptr2));
    figures_vector.push_back(static_cast<Figure*>(rectangle_ptr3));
    figures_vector.push_back(static_cast<Figure*>(square_ptr1));
    figures_vector.push_back(static_cast<Figure*>(square_ptr2));
    figures_vector.push_back(static_cast<Figure*>(square_ptr3));
    figures_vector.push_back(static_cast<Figure*>(trapeze_ptr1));
    figures_vector.push_back(static_cast<Figure*>(trapeze_ptr2));
    figures_vector.push_back(static_cast<Figure*>(trapeze_ptr3));

    figures_vector.print_geometric_center();
    figures_vector.print_area();
    figures_vector.print_perimeter();

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << figures_vector.calculate_total_area() << std::endl;

    figures_vector.erase(3);
    figures_vector.erase(7);

    figures_vector.print_geometric_center();
    figures_vector.print_area();
    figures_vector.print_perimeter();

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << figures_vector.calculate_total_area() << std::endl;

    figures_vector.pop_back();
    figures_vector.pop_back();

    figures_vector.print_geometric_center();
    figures_vector.print_area();
    figures_vector.print_perimeter();

    std::cout << std::endl << "Total area" << std::endl;
    std::cout << figures_vector.calculate_total_area() << std::endl;

    return 0;
}