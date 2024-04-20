#include "include/ParallelQuickSort.h"

int main(int argc, char** argv){
    if (argc < 2) {
        std::cerr << "Usage" << argv[0] << "<threads_num>" << std::endl;
    }

    int64_t threads_num = std::stoi(argv[1]);
    std::string test_data_file_name = "data_files/test_data.txt";
    std::vector<int64_t> vector;

    parallel_sort::ParallelQuickSort::fill_vector_from_file(vector, test_data_file_name);
    parallel_sort::ParallelQuickSort::parallel_quick_sort(vector, threads_num);

    std::cout << "Sorted arr: ";
    for (size_t i = 0; i != 10; ++i){
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl << "Size: " << vector.size() << std::endl;

    return 0;
}
