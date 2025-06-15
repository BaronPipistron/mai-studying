#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stack>
#include <vector>

int maxRectangleInHistogram(const std::vector<int>& heights) {
    std::stack<int> stack;
    int maxArea = 0;
    int n = heights.size();

    for (int i = 0; i <= n; ++i) {
        int h = (i == n ? 0 : heights[i]); 

        while (!stack.empty() && h < heights[stack.top()]) {
            int height = heights[stack.top()];
            stack.pop();
            int width = stack.empty() ? i : i - stack.top() - 1;
            maxArea = std::max(maxArea, height * width);
        }

        stack.push(i);
    }

    return maxArea;
}

int maxRectangleOfZeros(const std::vector<std::vector<int>>& matrix, int n, int m) {
    std::vector<int> heights(m, 0);
    int maxArea = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 0) {
                heights[j]++;
            } else {
                heights[j] = 0;
            }
        }
        
        maxArea = std::max(maxArea, maxRectangleInHistogram(heights));
    }

    return maxArea;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char ch;
            std::cin >> ch;
            matrix[i][j] = ch - '0';
        }
    }

    std::cout << maxRectangleOfZeros(matrix, n, m) << std::endl;
    return 0;
}