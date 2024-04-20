#include "../../include/allocator/Allocator.h"

using namespace allocator;

template <class _Tp, std::size_t CHUNK_SIZE>
Allocator<_Tp, CHUNK_SIZE>::Allocator() 
{
    static_assert(CHUNK_SIZE > 0);

    _chunks.resize(CHUNK_SIZE);
    _free_chunks = static_cast<void**>(operator new(sizeof(void*) * CHUNK_SIZE));

    auto it = _chunks.begin();
    for (size_type i = 0; i != CHUNK_SIZE; ++i) {
        _free_chunks[i] = &(*it++);
    }

    _free_chunks_count = CHUNK_SIZE;
}

template <class _Tp, std::size_t CHUNK_SIZE>
Allocator<_Tp, CHUNK_SIZE>::~Allocator() noexcept
{
    _chunks.clear();
    operator delete(_free_chunks);
}

template <class _Tp, std::size_t CHUNK_SIZE>
typename Allocator<_Tp, CHUNK_SIZE>::pointer Allocator<_Tp, CHUNK_SIZE>::allocate(size_type __n) 
{
    if (__n == 0) return nullptr;

    if (__n > _free_chunks_count) {
        throw std::bad_alloc();
    }

    _free_chunks_count -= __n;
    return static_cast<pointer>(_free_chunks[_free_chunks_count]);
}

template <class _Tp, std::size_t CHUNK_SIZE>
void Allocator<_Tp, CHUNK_SIZE>::deallocate(pointer __p, size_type __n)
{
    if (__p == nullptr || __n == 0) return;

    if (_free_chunks_count + __n > CHUNK_SIZE) {
        throw std::bad_alloc();
    }

    for (size_type i = 0; i != __n; ++i) {
        _free_chunks[_free_chunks_count + i] = __p + i;
    }
    _free_chunks_count += __n;
}

template <class _Tp, std::size_t CHUNK_SIZE>
template <class _Tp1, class... Args>
void Allocator<_Tp, CHUNK_SIZE>::construct(_Tp1* __p, Args&&...__args) 
{
    new (__p) _Tp1(std::forward<Args>(__args)...);
}

template <class _Tp, std::size_t CHUNK_SIZE>
void Allocator<_Tp, CHUNK_SIZE>::destroy(pointer __p)
{
    __p->~_Tp();
}
