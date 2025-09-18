#include <cstdlib>
#include <iostream>

void bubbleSort(float* arr, std::size_t size) {
    if (size < 2) return;

    for (std::size_t i = 0; i != size - 1; ++i) {
        for (size_t j = 0; j != size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                float tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main() {
    std::size_t size;
    std::cin >> size;

    float* arr = static_cast<float*>(std::malloc(sizeof(float) * size));
    
    for (std::size_t i = 0; i != size; ++i) {
        std::cin >> arr[i];
    }

    bubbleSort(arr, size);

    std::cout << std::scientific;
    std::cout.precision(6);

    for (std::size_t i = 0; i != size; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    std::free(arr);

    return 0;
}