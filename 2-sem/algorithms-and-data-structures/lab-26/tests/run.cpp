#include <iostream>

#include "../include/Deque.h"
#include "../src/benchmark.cpp"

int main(){
    Deque<int> deq;

    deq.push_back(21);
    deq.push_back(1);
    deq.push_back(10);
    deq.push_back(3);
    deq.push_back(9);
    deq.push_back(7);
    deq.push_back(2);
    deq.push_back(15);
    deq.push_back(19);
    deq.push_back(17);
    deq.push_back(16);
    deq.push_back(20);

    std::cout << "Before sorting: ";
    deq.print();
    deq.sort();

    std::cout << "\nAfter sorting: ";
    deq.print();

    bencmark();
}