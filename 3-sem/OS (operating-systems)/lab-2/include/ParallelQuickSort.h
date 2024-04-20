#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <vector>
#include <string>

namespace parallel_sort {

struct ThreadArgs {
    std::vector<int64_t>& _vec;
    int64_t _left;
    int64_t _right;
};

class ParallelQuickSort final {
public:
    static void parallel_quick_sort(std::vector<int64_t>&, const uint8_t);
    static void fill_vector_from_file(std::vector<int64_t>&, const std::string&);

private:
    [[nodiscard]] static int64_t partition(std::vector<int64_t>&, const int64_t, const int64_t) noexcept;
    static void quick_sort(std::vector<int64_t>&, const int64_t, const int64_t) noexcept;
    static void* thread_quick_sort(void*) noexcept;

    template <class T>
    static void swap(T&, T&) noexcept;

    static void save_metrics(const std::string&, const auto&, const uint8_t) noexcept;
};

}; // namespace parallel_sort
