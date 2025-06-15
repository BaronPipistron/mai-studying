#include <cstdint>
#include <iostream>
#include <vector>

using ull = unsigned long long;
using array_t = std::vector<std::pair<ull, ull>>;

const int MAX_VALUE_BYTE = 256;
const int BIT_MASK = 0xff;

void countingSort(array_t& data, const int byte) {
    size_t data_size = data.size();

    std::vector<int> counterArr(MAX_VALUE_BYTE, 0);
    array_t helperArr(data_size);

    for (const std::pair<ull, ull>& element: data) {
        ++counterArr[(element.first >> byte) & BIT_MASK];
    }

    for (size_t i = 1; i != MAX_VALUE_BYTE; ++i) {
        counterArr[i] += counterArr[i - 1];
    }

    for (int i = data_size - 1; i >= 0; --i) {
        helperArr[counterArr[(data[i].first >> byte) & BIT_MASK] - 1] = data[i];
        --counterArr[(data[i].first >> byte) & BIT_MASK]; 
    }

    data = std::move(helperArr);
}

void radixSort(array_t& data) {
    for (int byte = 0; byte != 64; byte += 4) {
        countingSort(data, byte);
    }
}

std::istream& operator >> (std::istream& is, std::pair<ull, ull>& pair) {
    is >> pair.first >> pair.second;

    return is;
}

std::ostream& operator << (std::ostream& os, const array_t& data) {
    for (const std::pair<ull, ull>& element: data) {
        os << element.first << "\t" << element.second << "\n";
    }

    return os;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    array_t data;

    std::pair<ull, ull> keyValue;
    while (std::cin >> keyValue) {
        data.emplace_back(keyValue);
    }

    if (data.size() == 0) {
        return 0;
    }

    radixSort(data);

    std::cout << data << std::endl;

    return 0;
}