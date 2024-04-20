#include <chrono>
#include <cstdlib>
#include <vector>

#include "../include/allocator/ListAllocator.h"
#include "../include/allocator/MacKuseyCarelsAllocator.h"

using namespace allocator;

size_t page_size = sysconf(_SC_PAGESIZE);

void benchmark() {
    void* list_memory = sbrk(10000 * page_size);
    void* MKC_memory = mmap(NULL, 1000 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    ListAllocator list_alloc(list_memory, 10000 * page_size);
    MacKuseyCarelsAllocator MKC_alloc(MKC_memory, 1000 * page_size);
    std::vector<void*> list_blocks;
    std::vector<void*> MKC_blocks;

    std::cout << "Comparing ListAllocator and MacKuseyCarelsAllocator" << std::endl;

    std::cout << "Block allocation rate" << std::endl;
    auto start_time = std::chrono::steady_clock::now();
    for (size_t i = 0; i != 100000; ++i) {
        void* block = list_alloc.alloc(i % 50 + 10);
        list_blocks.push_back(block);
    }
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "Time of alloc ListAllocator: " << 
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << 
                  " milliseconds" << std::endl;

    start_time = std::chrono::steady_clock::now();
    for (size_t i = 0; i != 100000; ++i) {
        void* block = MKC_alloc.alloc(i % 50 + 10);
        MKC_blocks.push_back(block);
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of alloc MacKuseyCarelsAllocator: " << 
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << 
                  " milliseconds" << std::endl;

    std::cout << "Block free rate" << std::endl;
    start_time = std::chrono::steady_clock::now();
    for (size_t i = 0; i != list_blocks.size(); ++i) {
        list_alloc.free(list_blocks[i]);
        if (i < 20) {
            std::cout << list_blocks[i] << std::endl;
        }
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of free ListAllocator: " << 
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << 
                  " milliseconds" << std::endl;

    start_time = std::chrono::steady_clock::now();
    for (size_t i = 0; i != MKC_blocks.size(); ++i) {
        MKC_alloc.free(MKC_blocks[i]);
        if (i < 20) {
            std::cout << MKC_blocks[i] << std::endl;
        }
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Time of free MacKuseyCarelsAllocator: " << 
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << 
                  " milliseconds" << std::endl;
}