#include "../include/List.h"

#include <chrono>
#include <iostream>
#include <forward_list>

void bencmark(){
    std::cout << "Comparing my \"List\" and \"forward_list\" from STL\n\n";

    List<int> my_list;
    std::forward_list<int> original_list;

    std::cout << "push_front\n";
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        my_list.push_front(i);
    }
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my List: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        original_list.push_front(i);
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original list: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "\npop_front\n";
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        my_list.pop_front();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my List: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        original_list.pop_front();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original list: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
}