#include "../include/Table.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

void benchmark(){
    std::cout << "Comparing my \"Sort\" and \"sort\" from STL\n\n";

    Table<std::string> test1;
    std::vector<std::string> test2;

    for (size_t i = 0; i != 1000000; ++i){
        test1.push_back("fka1fn" + std::to_string(i), "fmkdgsnrnr4312");
        test2.push_back("ekfaff" + std::to_string(i));
    }

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    test1.sort("zzzzzzzzzzzzzzzzzz");
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my Sort: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    std::sort(test2.begin(), test2.end());
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original sort: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
}