#include "../../include/allocator/MacKuseyCarelsAllocator.h"

using namespace allocator;

MacKuseyCarelsAllocator::MacKuseyCarelsAllocator(void_pointer real_memory, size_type memory_size)
{
    _memory = reinterpret_cast<void*>(reinterpret_cast<int8_t*>(real_memory) + sizeof(MacKuseyCarelsAllocator));
    _free_pages_list = nullptr;
    _memory_size = memory_size - sizeof(MacKuseyCarelsAllocator);
    _page_size = getpagesize();
}

MacKuseyCarelsAllocator::~MacKuseyCarelsAllocator()
{
    Page* cur_page = this->_free_pages_list;

    while (cur_page) {
        Page* to_delete = cur_page;
        cur_page = cur_page->_next;
        munmap(to_delete, _page_size);
        to_delete = nullptr;
    }
    _free_pages_list = nullptr;
}

typename Allocator::void_pointer MacKuseyCarelsAllocator::alloc(size_type new_block_size)
{
    if (_memory_size < new_block_size) return nullptr;

    size_t rounded_block_size = 1;
    while (rounded_block_size < new_block_size) {
        rounded_block_size *= 2;
    }

    Page* prev_page = nullptr;
    Page* cur_page = _free_pages_list;

    while (cur_page) {
        if (!cur_page->_is_large && cur_page->_block_size == rounded_block_size) {
            void_pointer block = reinterpret_cast<void_pointer>(cur_page);
            _free_pages_list = cur_page->_next;
            _memory_size -= new_block_size;

            return block;
        }

        prev_page = cur_page;
        cur_page = cur_page->_next;
    }

    if (_memory_size < _page_size) return nullptr;

    Page* new_page = reinterpret_cast<Page*>(mmap(NULL, _page_size, 
                                                  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 
                                                  -1, 0));
                                                
    if (new_page == MAP_FAILED) {
        throw std::bad_alloc();
    }
    new_page->_is_large = false;
    new_page->_block_size = rounded_block_size;
    new_page->_next = nullptr;

    size_t num_blocks = _page_size / rounded_block_size;
    for (size_t i = 0; i != num_blocks; ++i) {
        Page* block_page = reinterpret_cast<Page*>(reinterpret_cast<int8_t*>(new_page) + i * rounded_block_size);
        block_page->_is_large = false;
        block_page->_block_size = rounded_block_size;
        block_page->_next = this->_free_pages_list;
        this->_free_pages_list = block_page;
    }

    void_pointer block = reinterpret_cast<void_pointer>(new_page);
    this->_free_pages_list = new_page->_next;

    return block;
}

void MacKuseyCarelsAllocator::free(void_pointer block)
{
    if (block == nullptr) return;

    Page* page = reinterpret_cast<Page*>(block);
    page->_next = _free_pages_list;
    _free_pages_list = page;
}