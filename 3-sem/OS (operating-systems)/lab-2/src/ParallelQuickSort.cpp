#include "../include/ParallelQuickSort.h"

using namespace parallel_sort;

void ParallelQuickSort::parallel_quick_sort(
                                            std::vector<int64_t>& vector, 
                                            const uint8_t threads_num
                                           )
{
    if (threads_num < 1) {
        throw std::invalid_argument("Threads num must be mpre than zero");
    }

    std::vector<pthread_t> threads(threads_num);
    std::vector<ThreadArgs> thread_args;

    auto start_time = std::chrono::steady_clock::now();

    for (int64_t i = 0; i != threads_num; ++i) {
        int64_t left = i * (vector.size() / threads_num);
        int64_t right = (i == threads_num - 1) ? vector.size() - 1 : ((i + 1) * (vector.size() / threads_num)) - 1;

        ThreadArgs args { vector, left, right };
        thread_args.push_back(args);
    }

    for (int64_t i = 0; i != threads_num; ++i) {
        pthread_create(&threads[i], NULL, thread_quick_sort, &thread_args[i]);
    }

    for (int64_t i = 0; i != threads_num; ++i) {
        pthread_join(threads[i], NULL);
    }

    quick_sort(vector, 0, vector.size() - 1);

    auto end_time = std::chrono::steady_clock::now();

    std::string file_name = "data_files/metrics.txt";
    auto spent_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    
    save_metrics(file_name, spent_time, threads_num);
}

void ParallelQuickSort::fill_vector_from_file(
                                              std::vector<int64_t>& vector,
                                              const std::string& file_name
                                             )
{
    std::ifstream data_file(file_name);

    if (!data_file.is_open()) {
        throw std::runtime_error("Failed with opening file");
    }

    int64_t number;
    while (!data_file.eof()) {
        data_file >> number;
        vector.push_back(number);
    }
}

int64_t ParallelQuickSort::partition(
                                     std::vector<int64_t>& vector, 
                                     const int64_t left, 
                                     const int64_t right
                                    ) noexcept 
{
    int64_t pivot = vector[right];
    int64_t i = (left - 1);

    for (int64_t j = left; j <= right - 1; ++j) {
        if (vector[j] <= pivot) {
            ++i;
            swap(vector[i], vector[j]);
        }
    }

    swap(vector[i + 1], vector[right]);
    return (i + 1);
}

void ParallelQuickSort::quick_sort(
                                   std::vector<int64_t>& vector, 
                                   const int64_t left, 
                                   const int64_t right
                                  ) noexcept 
{
    if (left < right) {
        int64_t partition_i = partition(vector, left, right);

        quick_sort(vector, left, partition_i - 1);
        quick_sort(vector, partition_i + 1, right);
    }
}

void* ParallelQuickSort::thread_quick_sort(void* args) noexcept {
    ThreadArgs* thread_args = static_cast<ThreadArgs*>(args);

    std::vector<int64_t>& vector = thread_args->_vec;
    int64_t left = thread_args->_left;
    int64_t right = thread_args->_right;

    quick_sort(vector, left, right);
    return NULL;
}

template <class T>
void ParallelQuickSort::swap(T& first, T& second) noexcept {
    T tmp = first;
    first = second;
    second = tmp;
}

void ParallelQuickSort::save_metrics(
                                     const std::string& file_name,
                                     const auto& spent_time,
                                     const uint8_t threads_num
                                    ) noexcept 
{
    std::ofstream metrics_file(file_name, std::ios::app);
    metrics_file << static_cast<int>(threads_num) << ' ' << spent_time << std::endl;
}