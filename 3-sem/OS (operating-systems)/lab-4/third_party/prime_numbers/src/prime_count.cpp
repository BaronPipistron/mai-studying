#include "../include/prime_count.h"

int numbers::prime::naive_prime_count(int A, int B)
{
    if (B < A) {
        throw std::invalid_argument("naive_prime_count ERROR: first num "
                                    "must be less or equal than second num");
    }

    int counter { 0 };

    for (int i = A; i <= B; ++i) {
        if (i == 0 || i == 1) {
            continue;
        }

        int count_divider { 0 };
        for (int j = 2; j <= i; ++j) {
            if (i % j == 0) {
                ++count_divider;
            }
        }

        if (count_divider <= 1) {
            ++counter;
        }
    }

    return counter;
}

int numbers::prime::eratosphene_prime_count(int A, int B)
{
    if (B < A) {
        throw std::invalid_argument("naive_prime_count ERROR: first num "
                                    "must be less or equal than second num");
    }

    std::vector<bool> is_prime(B + 1, true);

    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= B; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= B; j += i) {
                is_prime[j] = false;
            }
        }
    }

    int counter = 0;
    for (int i = A; i <= B; ++i) {
        if (is_prime[i]) {
            ++counter;
        }
    }

    return counter;
}