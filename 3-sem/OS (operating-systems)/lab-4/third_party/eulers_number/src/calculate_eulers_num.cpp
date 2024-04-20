#include "../include/calculate_eulers_num.h"

float numbers::eulers::E(int x)
{
    if (x <= 0) {
        throw std::invalid_argument("E ERROR: given number "
                                    "must be more than zero");
    }

    return pow(1 + 1.0 / x, x);
}

float numbers::eulers::sum_of_series(int x)
{
    if (x <= 0) {
        throw std::invalid_argument("E ERROR: given number "
                                    "must be more than zero");
    }

    float sum { 1.0 };
    float factorial { 1.0 };
    
    for (int n = 1; n <= x; ++n) {
        factorial *= n;
        sum += 1.0 / factorial;
    }
    
    return sum;
}