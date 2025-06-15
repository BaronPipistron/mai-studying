#include <cstdint>
#include <iostream>
#include <limits>
#include <set>
#include <vector>


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    int32_t n;
    int32_t m;
    int32_t start;
    int32_t finish;

    std::cin >> n;
    std::cin >> m;
    std::cin >> start;
    std::cin >> finish;

    std::set<std::pair<int32_t, std::pair<int32_t, int32_t>>> graph;
    std::vector<int64_t> smallest_path_len(n + 1, INT64_MAX);

    smallest_path_len[start] = 0;

    int64_t u;
    int64_t v;
    int64_t weight;

    for (; std::cin >> u >> v >> weight;) {
        graph.insert({u, {v, weight}});
    }

    if (start == finish) {
        std::cout << 0;

        return 0;
    }

    for (int i = 0; i != n - 1; ++i) {
        bool flag = true;

        for (auto& data: graph) {
            if (smallest_path_len[data.first] == INT64_MAX) {
                continue;
            }

            if (smallest_path_len[data.second.first] > smallest_path_len[data.first] + data.second.second) {
                smallest_path_len[data.second.first] = smallest_path_len[data.first] + data.second.second;
                flag = false;
            }
        }

        if (flag) {
            break;
        }
    }

    if (smallest_path_len[finish] == INT64_MAX) {
        std::cout << "No solution" << std::endl;
    } else {
        std::cout << smallest_path_len[finish] << std::endl;
    }

    return 0;
}