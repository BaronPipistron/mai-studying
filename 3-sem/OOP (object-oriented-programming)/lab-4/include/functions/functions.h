#pragma once

#include "../containers/vector.h"
#include "../interfaces/figure.h"

using namespace containers;
using namespace figures;

namespace functions {

double calculate_total_area(vector<std::shared_ptr<Figure<double>>>&);

void print_area(vector<std::shared_ptr<Figure<double>>>&);

void print_geometric_center(vector<std::shared_ptr<Figure<double>>>&);

void print_perimeter(vector<std::shared_ptr<Figure<double>>>&);

}; // namespace functions