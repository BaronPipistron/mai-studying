#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

inline bool is_valid_triangle(const uint64_t a, const uint64_t b, const uint64_t c) noexcept {
    return (a + b > c && a + c > b && b + c > a) ? true : false;
}

double calculate_triangle_area(const uint64_t a, const uint64_t b, const uint64_t c) noexcept {
    double semi_perimeter = (a + b + c) * 0.5;
    double triangle_area = std::sqrt(
        semi_perimeter * (semi_perimeter - a) * (semi_perimeter - b) * (semi_perimeter - c)
        );

    return triangle_area;
}

int main() {
    uint32_t amount_of_sections;
    std::cin >> amount_of_sections;

    std::vector<uint64_t> sections;
    for (uint64_t section; std::cin >> section; ) {
        sections.push_back(section);
    }

    std::sort(sections.begin(), sections.end());

    uint64_t long_section;
    uint64_t middle_section;
    uint64_t short_section;

    double max_triangle_area = 0;

    for (size_t i = sections.size() - 1; i > 1; --i) {
        uint64_t a = sections[i];
        uint64_t b = sections[i - 1];
        uint64_t c = sections[i - 2];

        if (is_valid_triangle(a, b, c)) {
            double triangle_area = calculate_triangle_area(a, b, c);

            if (triangle_area > max_triangle_area) {
                max_triangle_area = triangle_area;

                long_section = a;
                middle_section = b;
                short_section = c;
            }
        }
    }

    if (max_triangle_area != 0) {
        std::cout.precision(3);
        std::cout << std::fixed << max_triangle_area << std::endl;

        std::cout << short_section << ' ' << middle_section << ' ' << long_section << std::endl;
    } else {
        std::cout << 0 << std::endl;
    }

    return 0;
}