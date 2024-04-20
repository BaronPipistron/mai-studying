#include "include/allocator/ListAllocator.h"
#include "include/allocator/MacKuseyCarelsAllocator.h"
#include "src/benchmarks/benchmark.cpp"

using namespace allocator;

int main() {
    size_t page_size = sysconf(_SC_PAGESIZE);
    void* list_memory = sbrk(10 * page_size);

    ListAllocator list_alloc(list_memory, 10 * page_size);

    void* MKC_memory = mmap(NULL, 10 * page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    MacKuseyCarelsAllocator MKC_alloc(MKC_memory, 10 * page_size);

    void* block1 = list_alloc.alloc(10000);
    void* block2 = list_alloc.alloc(10);
    void* block3 = list_alloc.alloc(44651047871);

    void* block4 = MKC_alloc.alloc(1024);
    void* block5 = MKC_alloc.alloc(2049);
    void* block6 = MKC_alloc.alloc(144420166);

    printf("Block 1: %p\n", block1);
    printf("Block 2: %p\n", block2);
    printf("Block 3: %p\n", block3);

    printf("Block 4: %p\n", block4);
    printf("Block 5: %p\n", block5);
    printf("Block 6: %p\n", block6);

    benchmark();

    return 0;
}

