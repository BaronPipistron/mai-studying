#pragma once

#include <cstddef>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace allocator {

template <class _Tp, std::size_t CHUNK_SIZE = 1024>
class Allocator {
  public:
    using value_type = _Tp;
    using pointer = _Tp*;
    using const_pointer = const _Tp*;
    using reference = _Tp&;
    using const_reference = const _Tp&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    template <class _Tp1>
    struct rebind {
        using other = Allocator<_Tp1, CHUNK_SIZE>;
    };

  private:
    std::deque<value_type> _chunks;
    size_type _free_chunks_count;
    void** _free_chunks;

  public:
    Allocator();
    virtual ~Allocator() noexcept;

    pointer allocate(size_type);
    void deallocate(pointer, size_type);

    template <class _Tp1, class... Args>
    void construct(_Tp1*, Args&&...);

    void destroy(pointer);
};

template <class _Tp, class _Tp1>
constexpr bool operator==(const Allocator<_Tp>& lhs, const Allocator<_Tp1>& rhs) noexcept
{
    return (&lhs == &rhs);
}

template <class _Tp, class _Tp1>
constexpr bool operator!=(const Allocator<_Tp>& lhs, const Allocator<_Tp1>& rhs) noexcept
{
    return (&lhs != &rhs);
}

}; // namespace allocator
