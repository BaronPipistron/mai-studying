#include <cmath>
#include <iostream>

inline bool isZero(float num, float eps = 1e-12f) {
    return std::fabs(num) <= eps; 
}

// a*x^2 + b*x + c = 0
// Возможные случаи:
// 1) 2 корня - D > 0
// 2) 1 корень - D = 0
// 3) Мнимые корни - D < 0
// 4) Бесконечно много корней - a = 0, b = 0, c = 0
// 5) Нет корней

int main() {
    std::cout.precision(6);

    float a, b, c;
    float x_1, x_2;

    std::cin >> a >> b >> c;

    if (isZero(a)) {
        // b*x + c = 0 => x = -c/b
        if (isZero(b)) {
            // c = 0
            if (isZero(c)) {
                std::cout << "any" << std::endl;
                return 0;
            }

            std::cout << "incorrect" << std::endl;
            return 0;
        }

        x_1 = -c / b;
        std::cout << std::fixed << x_1 << std::endl;
        return 0;
    }

    float D = b*b - 4.0f*a*c;
    float denominator = 2.0f*a;

    if (isZero(D)) {
        // x = -b/2a
        x_1 = -b / denominator;
        std::cout << std::fixed << x_1 << std::endl;
    } else if (D > 0) {
        float sqrtD = std::sqrt(D);

        x_1 = (-b + sqrtD) / denominator;
        x_2 = (-b - sqrtD) / denominator;

        std::cout << std::fixed << x_1 << ' ' << x_2 << std::endl;
    } else {
        std::cout << "imaginary" << std::endl;
    }

    return 0;
}
