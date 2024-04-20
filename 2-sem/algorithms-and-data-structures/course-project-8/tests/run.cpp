#include <iostream>

#include "../src/benchmark.cpp"
#include "../include/List.h"

int main(){
    List<int> lst;

    lst.push_back(10);
    lst.push_back(15);
    lst.push_back(16);
    lst.push_back(12);
    lst.push_back(18);
    lst.push_front(5);
    lst.push_front(20);

    lst.swap_halfs();
    std::cout << lst << "\n";

    bencmark();
}