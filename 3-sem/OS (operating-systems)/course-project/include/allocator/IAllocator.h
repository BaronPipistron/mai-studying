#pragma once

#include <exception>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>

namespace allocator {

class Allocator {
  public:
    using void_pointer = void*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

  protected:
    Allocator() = default;

  public:
    virtual ~Allocator() = default;

    virtual void_pointer alloc(const size_type) = 0;
    virtual void free(void_pointer) = 0;
};

}; // namespace allocator