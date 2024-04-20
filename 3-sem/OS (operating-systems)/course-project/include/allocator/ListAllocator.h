#pragma once

#include "IAllocator.h"

namespace allocator {

struct BlockHeader {
    size_t _size;
    BlockHeader* _next;
};

class ListAllocator final : public Allocator {
  private:
    BlockHeader* _free_blocks_list;

  public:
    ListAllocator() = delete;
    ListAllocator(void_pointer, size_type);

    virtual ~ListAllocator();

    virtual void_pointer alloc(size_type) override;
    virtual void free(void_pointer) override;
};

}; // namespace allocator