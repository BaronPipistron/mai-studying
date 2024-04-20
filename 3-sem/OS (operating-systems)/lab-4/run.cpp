#include <iostream>

#include "third_party/eulers_number/include/calculate_eulers_num.h"
#include "third_party/prime_numbers/include/prime_count.h"

int main(int argc, char** argv) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Invalid input data!\nExample:\nmake mode=1 a=1 b=1\nor\nmake run mode=2 a=1" << std::endl;
        return -1;
    }

    int mode = std::atoi(argv[1]);

    if (mode != 1 && mode != 2) {
        std::cerr << "Invalid data!\nValid value of variable \"mode\" is 1 or 2" << std::endl;
        return -1;
    }

    int a = std::atoi(argv[2]);
    int b = std::atoi(argv[3]);

    switch (mode) {
        case 1:
            std::cout << "Your choice: primes count" << std::endl;
            std::cout << "Naive implement: " << numbers::prime::naive_prime_count(a, b) << std::endl;
            std::cout << "Eratosthenes method: " << numbers::prime::eratosphene_prime_count(a, b) << std::endl;
            return 0;
    
        case 2:
            std::cout << "Your choice: calculate Euler's number" << std::endl;
            std::cout << "(1 + 1 / x) ^ x method: " << numbers::eulers::E(a) << std::endl;
            std::cout << "Sum of series method: " << numbers::eulers::sum_of_series(a) << std::endl;
            return 0;
  }

}