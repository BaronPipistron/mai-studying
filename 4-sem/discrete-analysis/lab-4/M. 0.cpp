#include <iostream>
#include <string>
#include <vector>

void calculateZFunc(std::vector<int>& zArray, const std::string& str) {
    for (int i = 1; i != str.length(); ++i) {
        int left = 0;
        int right = 0;

        if (i <= right) {
            zArray[i] = std::min(right - i + 1, zArray[i - left]);
        }

        while (i + zArray[i] < str.length() && str[zArray[i]] == str[i + zArray[i]]) {
            ++zArray[i];
        }

        if (i + zArray[i] - 1 > right) {
            left = i;
            right = i + zArray[i] - 1;
        }
    }
}

int main() {
    std::string text;
    std::string pattern;

    std::cin >> text >> pattern;

    std::string stringToFunction = pattern + "$" + text;
    std::vector<int> zArray (stringToFunction.length());

    calculateZFunc(zArray, stringToFunction);

    size_t patternLength = pattern.length();
    for (size_t i = patternLength; i != zArray.size(); ++i) {
        if (zArray[i] == patternLength) {
            std::cout << i - patternLength - 1 << std::endl;
        }
    }

    return 0;
}