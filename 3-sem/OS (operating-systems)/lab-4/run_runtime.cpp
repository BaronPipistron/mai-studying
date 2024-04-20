#include <iostream>
#include <dlfcn.h>

int (*naive_prime_count)(int, int);
int (*eratosphene_prime_count)(int, int);
float (*E)(int);
float (*sum_of_series)(int);

bool init_lib() {
    void* hdl_prime_lib = dlopen("/home/baronpipistron/MAI_OS/4_Lab/build/libprime_numbers.so", RTLD_LAZY);
    void* hdl_eulers_lib = dlopen("/home/baronpipistron/MAI_OS/4_Lab/build/libeulers.so", RTLD_LAZY);

    if (hdl_prime_lib == nullptr) {
        std::cerr << "init_lib ERROR: hdl_prime_lib is nullptr" << std::endl;
        return false;
    }

    if (hdl_eulers_lib == nullptr) {
        std::cerr << "init_lib ERROR: hdl_eulers_lib is nullptr" << std::endl;
        return false;
    }

    naive_prime_count = (int (*)(int, int)) dlsym(hdl_prime_lib, "_ZN7numbers5prime17naive_prime_countEii");
    eratosphene_prime_count = (int (*)(int, int)) dlsym(hdl_prime_lib, "_ZN7numbers5prime23eratosphene_prime_countEii");
    E = (float (*)(int)) dlsym(hdl_eulers_lib, "_ZN7numbers6eulers1EEi");
    sum_of_series = (float (*)(int)) dlsym(hdl_eulers_lib, "_ZN7numbers6eulers13sum_of_seriesEi");

    if (naive_prime_count == nullptr) {
        std::cerr << "init_lib ERROR: naive_prime_count is nullptr" << std::endl;
        return false;
    }

    if (eratosphene_prime_count == nullptr) {
        std::cerr << "init_lib ERROR: eratosphene_prime_count is nullptr" << std::endl;
        return false;
    }

    if (E == nullptr) {
        std::cerr << "init_lib ERROR: E is nullptr" << std::endl;
        return false;
    }

    if (sum_of_series == nullptr) {
        std::cerr << "init_lib ERROR: sum_of_series if nullptr" << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    if (argc < 3 || argc > 4) {
        std::cerr << "invalid arguments!" << std::endl;
        return -1;
    }

    bool check_flag = init_lib();
        if (check_flag == false) {
        std::cerr << "Error with open libs" << std::endl;
        return -1;
    }

    int mode = std::atoi(argv[1]);
    int a = std::atoi(argv[2]);
    int b = std::atoi(argv[3]);
    int impl_flag = 0;

    switch (mode) {
        case 2:
            std::cout << "Input implementation flag(0 - (1 + 1 / x) ^ x, oth - sum of series): ";
            std::cin >> impl_flag;
      
            if (impl_flag == 0) {
                std::cout << "(1 + 1 / x) ^ x method implementation: ";
                std::cout << E(a) << std::endl;
            } else {
                std::cout << "sum of series method: ";
                std::cout << sum_of_series(a) << std::endl;
             }

            break;

        case 1:
            std::cout << "Input implementation flag(0 - naive, oth - eratosphene): ";
            std::cin >> impl_flag;
      
            if (impl_flag == 0) {
                std::cout << "naive primes count implementation: ";
                std::cout << naive_prime_count(a, b) << std::endl;
            } else {
                std::cout << "eratosthenes primes count implementation: ";
                std::cout << eratosphene_prime_count(a, b) << std::endl;
            }
            break;

        default:
            std::cerr << "Invalid flag of mode!" << std::endl;
            break;
  }

  return 0;
}