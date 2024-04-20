#include "../include/Deque.h"
#include <chrono>
#include <deque>
#include <iostream>

void bencmark(){
    std::cout << "Comparing my \"Deque\" and \"deque\" from STL\n\n";

    Deque<int> my_deque;
    std::deque<int> original_deque;

    std::cout << "push_front\n";
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        my_deque.push_front(i);
    }
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my Deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        original_deque.push_front(i);
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "\npop_front\n";
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        my_deque.pop_front();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my Deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        original_deque.pop_front();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "\npush_back\n";
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        my_deque.push_back(i);
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my Deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        original_deque.push_back(i);
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "\npop_back\n";
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        my_deque.pop_back();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my Deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != 1000000; ++i){
        original_deque.pop_back();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "\nclear\n";
    start_time = std::chrono::steady_clock::now();
    my_deque.clear();
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my Deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    start_time = std::chrono::steady_clock::now();
    original_deque.clear();
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original deque: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    return;
}