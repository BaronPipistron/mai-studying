#include "../include/MyVector.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

void benchmark(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ifstream tests_file("../tests/test.txt", std::ios::in);

    if (!tests_file.is_open()){
        std::cerr << "ERROR OPENING FILE\n";
        return;
    }

    myVector<int> my_vector;
    std::vector<int> original_vector;
    int amount_of_numbers, number;

    std::cout << "\nComparing \"myVector\" and the \"vector\" from STL\n\n";
    tests_file >> amount_of_numbers;
    std::cout << "Amount of numbers: " << amount_of_numbers << "\n\n";

    std::cout << "push_back, complexity O(1)\n";
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != amount_of_numbers; ++i){
        tests_file >> number;
        my_vector.push_back(number);
    }
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    tests_file.clear();
    tests_file.seekg(0);
    tests_file >> number;
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != amount_of_numbers; ++i){
        tests_file >> number;
        original_vector.push_back(number);
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    if (my_vector.size() != original_vector.size()){
        throw std::exception();
    }

    for (size_t i = 0; i != original_vector.size(); ++i){
        if (my_vector[i] != original_vector[i]){
            std::cout << "ERROR: index " << i << " my_vector[i] = " << my_vector[i] <<
                    ", original_vector[i] = " << original_vector[i] << "\n";
            throw std::exception();
        }
    }
    
    std::cout << "pop_back, complexity O(1)\n";
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != amount_of_numbers; ++i){
        my_vector.pop_back();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != amount_of_numbers; ++i){
        original_vector.pop_back();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    if (my_vector.size() != original_vector.size()){
        throw std::exception();
    }

    for (size_t i = 0; i != original_vector.size(); ++i){
        if (my_vector[i] != original_vector[i]){
            std::cout << "ERROR: index " << i << " my_vector[i] = " << my_vector[i] <<
                    ", original_vector[i] = " << original_vector[i] << "\n";
            throw std::exception();
        }
    }

    std::cout << "shrink_to_fit\n";
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != amount_of_numbers; ++i){
        my_vector.shrink_to_fit();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i != amount_of_numbers; ++i){
        original_vector.shrink_to_fit();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    if (my_vector.capacity() != original_vector.capacity()){
        std::cout << "ERROR: my_vector.capacity() = " << my_vector.capacity() << 
                ", original_vector.capacity() = " << original_vector.capacity() << "\n";
    }

    std::cout << "reserve\n";
    start_time = std::chrono::steady_clock::now();
    my_vector.reserve(2 * amount_of_numbers);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    start_time = std::chrono::steady_clock::now();
    original_vector.reserve(2 * amount_of_numbers);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    std::cout << "my_vector.capacity() = " << my_vector.capacity() <<
        ", original_vector.capacity() = " << original_vector.capacity() << "\n";
    
    std::cout << "resize\n";
    start_time = std::chrono::steady_clock::now();
    my_vector.resize(2 * amount_of_numbers, number);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    original_vector.resize(2 * amount_of_numbers, number);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";
    
    if (my_vector.size() != original_vector.size()){
        throw std::exception();
    }

    for (size_t i = 0; i != original_vector.size(); ++i){
        if (my_vector[i] != original_vector[i]){
            std::cout << "ERROR: index " << i << " my_vector[i] = " << my_vector[i] << 
                ", original_vector[i] = " << original_vector[i] << "\n";
            throw std::exception();
        }
    }
    std::cout << "my_vector.capacity() = " << my_vector.capacity() << 
        ", original_vector.capacity() = " << original_vector.capacity() << "\n";
    std::cout << "my_vector.size() = " << my_vector.size() << 
        ", original_vector.size() = " << original_vector.size() << "\n";
    
    std::cout << "clear\n";
    start_time = std::chrono::steady_clock::now();
    my_vector.clear();
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    original_vector.clear();
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    if (my_vector.size() != 0 && original_vector.size() != 0){
        throw std::exception();
    }
    std::cout << "my_vector.capacity() = " << my_vector.capacity() << 
        ", original_vector.capacity() = " << original_vector.capacity() << "\n";
    std::cout << "my_vector.size() = " << my_vector.size() << 
        ", original_vector.size() = " << original_vector.size() << "\n";

    std::cout << "copy constructor\n";
    start_time = std::chrono::steady_clock::now();
    myVector<int> my_vector1(my_vector);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    std::vector<int> original_vector1(original_vector);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "equality\n";
    start_time = std::chrono::steady_clock::now();
    std::cout << (my_vector == my_vector1) << "\n";
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    std::cout << (original_vector == original_vector1) << "\n";
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    std::cout << "swap\n";
    start_time = std::chrono::steady_clock::now();
    my_vector.swap(my_vector1);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work my_vector: " << 
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    start_time = std::chrono::steady_clock::now();
    original_vector.swap(original_vector1);
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of work original_vector: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds\n";

    return;
}