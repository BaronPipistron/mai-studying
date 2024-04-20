#include "../../include/containers/vector.h"

using namespace containers;

template <class T, class Allocator>
vector<T, Allocator>::vector(): 
    _size(0), _capacity(1), _data(AllocatorTraits::allocate(_allocator, 1))
{
}

template <class T, class Allocator>
vector<T, Allocator>::vector(Allocator& allocator):
    _size(0), _capacity(1), _data(AllocatorTraits::allocate(_allocator, 1)), _allocator(allocator)
{
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const size_t capacity):
    _size(0), _capacity(capacity), _data(AllocatorTraits::allocate(_allocator, capacity))
{ 
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const size_t size, const T& value) :
    vector()
{
    resize(size, value);
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const std::initializer_list<T>& list) {
    T* new_data = AllocatorTraits::allocate(_allocator, list.size());

    try {
        if (std::is_copy_constructible<T>::value) {
            std::uninitialized_copy(list.begin(), list.end(), new_data);
        } else {
            std::uninitialized_move(list.begin(), list.end(), new_data);
        }
    } catch(...) {
        AllocatorTraits::deallocate(_allocator, new_data, list.size());
        throw;
    }

    this->_data = new_data;
    this->_size = list.size();
    this->_capacity = list.size();
}

template <class T, class Allocator>
vector<T, Allocator>::vector(const vector<T, Allocator>& other) noexcept :
    _size(other._size), _capacity(other._capacity), _allocator(other._allocator)
{
    T* new_data = AllocatorTraits::allocate(_allocator, _capacity);

    try {
        std::uninitialized_copy(other._data, other._data + other._size, new_data);
    } catch(...) {
        AllocatorTraits::deallocate(_allocator, new_data, _capacity);
        throw;
    }

    _data = new_data;
}

template <class T, class Allocator>
vector<T, Allocator>::vector(vector<T, Allocator>&& other) noexcept :
    _size(other._size), _capacity(other._capacity), _data(other._data), _allocator(std::move(other._allocator))
{
    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(const vector<T, Allocator>& other) noexcept {
    if (this == &other) return *this;

    clear();
    AllocatorTraits::deallocate(_allocator, _data, _capacity);

    this->_size = other._size;
    this->_capacity = other._capacity;
    this->_data = AllocatorTraits::allocate(_allocator, _capacity);

    try {
        std::uninitialized_copy(other._data, other._data + other._size, _data);
    } catch(...) {
        AllocatorTraits::deallocate(_allocator, _data, _capacity);
        throw;
    }

    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>& vector<T, Allocator>::operator=(vector<T, Allocator>&& other) noexcept {
    if (this == &other) return *this;

    clear();
    AllocatorTraits::deallocate(_allocator, _data, _capacity);

    this->_size = std::move(other._size);
    this->_capacity = std::move(other._capacity);
    this->_data = std::move(other._data);
    this->_allocator = std::move(other._allocator);

    other._size = 0;
    other._capacity = 0;
    other._data = nullptr;

    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>::~vector() noexcept {
    for (size_t i = 0; i != _size; ++i) {
        _data[i].~T();
    }

    AllocatorTraits::deallocate(_allocator, _data, _capacity);
    _data = nullptr;
}

template <class T, class Allocator>
void vector<T, Allocator>::reserve(const size_t new_capacity) {
    if (new_capacity <= _capacity) return;

    T* new_data = AllocatorTraits::allocate(_allocator, new_capacity);
    try {
        std::uninitialized_copy(_data, _data + _size, new_data);
    } catch(...) {
        AllocatorTraits::deallocate(_allocator, new_data, new_capacity);
        throw;
    }

    for (size_t i = 0; i != _size; ++i) {
        _data[i].~T();
    }

    AllocatorTraits::deallocate(_allocator, _data, _capacity);
    _data = new_data;
    _capacity = new_capacity;
}

template <class T, class Allocator>
void vector<T, Allocator>::resize(const size_t new_size, const T& value) {
    if (new_size > _capacity) reserve(new_size);

    if (new_size > _size) {
        for (size_t i = _size; i != new_size; ++i) {
            AllocatorTraits::construct(_allocator, _data + i, std::move(value));
        }
    }

    if (new_size < _size) {
        for (size_t i = new_size; i != _size; ++i) {
            _data[i].~T();
        }
    }

    this->_size = new_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::shrink_to_fit() {
    if (_size == 0) {
        reserve(1);
        return;
    }

    reserve(_size);
}

template <class T, class Allocator>
void vector<T, Allocator>::clear() {
    for (size_t i = 0; i != _size; ++i) {
        _data[i].~T();
    }

    _size = 0;
}

template <class T, class Allocator>
void vector<T, Allocator>::push_back(const T& value) {
    if (_capacity == _size) {
        if (_capacity * 2 < _size) {
            throw std::range_error("Capacity overflow");
        }

        reserve(2 * _capacity);
    }

    AllocatorTraits::construct(_allocator, _data + _size, std::move(value));
    ++_size;
}

template <class T, class Allocator>
void vector<T, Allocator>::pop_back() noexcept {
    if (_size == 0) return;

    --_size;
    _data[_size].~T();
}

template <class T, class Allocator>
void vector<T, Allocator>::erase(const size_t index) {
    if (index >= _size) throw std::range_error("Index out of range");
    if (_size == 0) return;

    if (index == _size - 1) {
        pop_back();
        return;
    }

    _data[index].~T();
    for (size_t i = index; i != _size - 1; ++i) {
        _data[i] = _data[i + 1];
    }
    pop_back();
}

template <class T, class Allocator>
template <class... Args>
void vector<T, Allocator>::emplace_back(const Args& ...args) {
    if (_capacity == _size) {
        if (_capacity * 2 < _size) {
            throw std::range_error("Capacity overflow");
        }

        reserve(2 * _capacity);
    }

    AllocatorTraits::construct(_allocator, _data + _size, std::move(args...));
    ++_size;
}

template <class T, class Allocator>
size_t vector<T, Allocator>::size() const noexcept {
    return _size;
}

template <class T, class Allocator>
size_t vector<T, Allocator>::capacity() const noexcept {
    return _capacity;
}

template <class T, class Allocator>
T& vector<T, Allocator>::operator[](const size_t index) noexcept {
    return _data[index];
}

template <class T, class Allocator>
T& vector<T, Allocator>::at(const size_t index) {
    if (index >= _size) {
        throw std::range_error("Index out of range");
    }

    return _data[index];
}

template <class T, class Allocator>
T& vector<T, Allocator>::front() {
    if (_size == 0) {
        throw std::range_error("Vector is empty");
    }

    return _data[0];
}

template <class T, class Allocator>
T& vector<T, Allocator>::back() {
    if (_size == 0) {
        throw std::range_error("Vector is empty");
    }

    return _data[_size - 1];
}

template <class T, class Allocator>
const T& vector<T, Allocator>::operator[](const size_t index) const noexcept {
    return _data[index];
}


template <class T, class Allocator>
const T& vector<T, Allocator>::at(const size_t index) const {
    if (index >= _size) {
        throw std::range_error("Index out of range");
    }

    return _data[index];
}

template <class T, class Allocator>
const T& vector<T, Allocator>::front() const {
    if (_size == 0) {
        throw std::range_error("Vector is empty");
    }

    return _data[0];
}

template <class T, class Allocator>
const T& vector<T, Allocator>::back() const {
    if (_size == 0) {
        throw std::range_error("Vector is empty");
    }

    return _data[_size - 1];
}

template <class T, class Allocator>
bool vector<T, Allocator>::empty() const noexcept {
    return (_size == 0);
}

template <class T, class Allocator>
bool vector<T, Allocator>::operator==(const vector<T, Allocator>& other) const noexcept {
    if (this->_size != other._size) return false;

    for (size_t i = 0; i != this->_size; ++i) {
        if (this->_data[i] != other._data[i]) {
            return false;
        }
    }

    return true;
}

template <class T, class Allocator>
bool vector<T, Allocator>::operator!=(const vector<T, Allocator>& other) const noexcept {
    return !(*this == other);
}

template <class T, class Allocator>
vector<T, Allocator>::iterator::iterator(T* ptr):
    _ptr(ptr)
{
}

template <class T, class Allocator>
vector<T, Allocator>::iterator::iterator(const iterator& other):
    _ptr(other._ptr)
{ 
}

template <class T, class Allocator>
T& vector<T, Allocator>::iterator::operator*() noexcept {
    return *_ptr;
}

template <class T, class Allocator>
const T& vector<T, Allocator>::iterator::operator*() const noexcept {
    return *_ptr;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator& vector<T, Allocator>::iterator::operator++() {
    this->_ptr += 1;
    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator& vector<T, Allocator>::iterator::operator--() {
    this->_ptr -= 1;
    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator vector<T, Allocator>::iterator::operator++(int) {
    iterator tmp(this->_ptr);
    this->_ptr += 1;
    return *tmp;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator vector<T, Allocator>::iterator::operator--(int) {
    iterator tmp(this->_ptr);
    this->_ptr -= 1;
    return *tmp;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator& vector<T, Allocator>::iterator::operator+=(const int64_t number) {
    this->_ptr += number;
    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator& vector<T, Allocator>::iterator::operator-=(const int64_t number) {
    this->_ptr -= number;
    return *this;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator vector<T, Allocator>::iterator::operator+(const int64_t number) const {
    iterator sum(this->_ptr);
    return sum += number;
}

template <class T, class Allocator>
vector<T, Allocator>::iterator vector<T, Allocator>::iterator::operator-(const int64_t number) const {
    iterator sum(this->_ptr);
    return sum -= number;
}

template <class T, class Allocator>
bool vector<T, Allocator>::iterator::operator==(const vector<T, Allocator>::iterator& other) const noexcept {
    return (this->_ptr == other._ptr);
}

template <class T, class Allocator>
bool vector<T, Allocator>::iterator::operator!=(const vector<T, Allocator>::iterator& other) const noexcept {
    return !(this->_ptr == other._ptr);
}