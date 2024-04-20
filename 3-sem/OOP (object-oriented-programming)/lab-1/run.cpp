#include "include/task.hpp"
#include <iostream>

using namespace MAI::OOP::Lab1;

int main() {
    int64_t input_num;
    std::cout << "Input the number: ";
    std::cin >> input_num;

    if (input_num < 0) {
        throw std::invalid_argument("Input number must be bigger or equal zero");
    }

    Solution::task(input_num);
    
    return 0;
}