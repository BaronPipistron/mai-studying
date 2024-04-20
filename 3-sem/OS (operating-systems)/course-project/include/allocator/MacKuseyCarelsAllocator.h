#pragma once

#include "IAllocator.h"

namespace allocator {

struct Page {
    Page* _next;
    bool _is_large;
    size_t _block_size;
};

class MacKuseyCarelsAllocator final : public Allocator {
  private:
    void* _memory;
    Page* _free_pages_list;
    size_t _memory_size;
    size_t _page_size;

  public:
    MacKuseyCarelsAllocator() = delete;
    MacKuseyCarelsAllocator(void_pointer, size_type);

    virtual ~MacKuseyCarelsAllocator();

    virtual void_pointer alloc(size_type) override;
    virtual void free(void_pointer) override;
};

}; // namespace allocator