#include "../include/task.hpp"
#include <iostream>

using namespace MAI::OOP::Lab1;

std::string Solution::task(uint32_t num) {
    std::string res {""};

    while (num > 0) {
        res.insert(res.begin(), '0' + (num & 1));
        num >>= 1;
    }

    while (res.length() < 32) {
        res.insert(res.begin(), '0');
    }

    std::cout << "The resulting number in binary notation: " << res << std::endl;
    return res;
}