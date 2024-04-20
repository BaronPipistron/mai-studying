#include <iostream>
#include "include/Twelve.hpp"

using namespace MAI::OOP::Lab2;

int main() {
    Twelve a("00");
    Twelve b("0");
    Twelve c = a + b;

    std::cout << c << std::endl;

    return 0;
}