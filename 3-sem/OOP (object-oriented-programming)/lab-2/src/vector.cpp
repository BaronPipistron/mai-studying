#include "../include/vector.hpp"
#include <climits>
#include <memory>

using namespace MAI::OOP::Lab2;

template <typename T>
vector<T>::vector(): _vec_size(0), _vec_capacity(1) {
    _arr = reinterpret_cast<T*>(new int8_t[sizeof(T)]);
}

template <typename T>
vector<T>::vector(const size_t start_size, const T& val) {
    resize(start_size, val);
}

template <typename T>
vector<T>::vector(const std::initializer_list<T>& init_list): vector() {
    T* _new_arr = reinterpret_cast<T*>(new int8_t[sizeof(T) * init_list.size()]);

    try {
        std::uninitialized_copy(init_list.begin(), init_list.end(), _new_arr);
    } catch(...) {
        delete[] reinterpret_cast<int8_t*>(_new_arr);
        throw;
    }

    delete[] reinterpret_cast<int8_t*>(_arr);
    _arr = _new_arr;
    _vec_size = init_list.size();
    _vec_capacity = init_list.size();
}

template <>
inline vector<unsigned char>::vector(const std::string& str): vector() {
    for (char c: str) {
        push_back(c);
    }

    _vec_size = str.length();
    _vec_capacity = str.length();
}

template <typename T>
vector<T>::vector(const vector<T>& other_vector): vector() {
    resize(other_vector.size());

    for (size_t i = 0; i != this->_vec_size; ++i) {
        this->_arr[i].~T();
        new (this->_arr + i) T(other_vector[i]);
    }
}

template <typename T>
vector<T>::vector(vector<T>&& other) noexcept {
    this->_vec_size = std::move(other._vec_size);
    this->_vec_capacity = std::move(other._vec_capacity);
    this->_arr = std::move(other._arr);

    other._vec_size = 0;
    other._vec_capacity = 1;
    other._arr = nullptr;
}

template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& right_vector) {
    if (this == &right_vector) return *this;
    
    resize(right_vector.size());
    for (size_t i = 0; i != this->_vec_size; ++i) {
        this->_arr[i].~T();
        new (this->_arr + i) T(right_vector[i]);
    }
    return *this;
}

template <typename T>
void vector<T>::reserve(const size_t new_capacity) {
    if (new_capacity > ULLONG_MAX) throw std::range_error("Capacity overflow");
    if (new_capacity <= _vec_capacity) return;

    T* _new_arr = reinterpret_cast<T*>(new int8_t[sizeof(T) * new_capacity]);
    try {
        std::uninitialized_copy(_arr, _arr + _vec_size, _new_arr);
    } catch(...) {
        delete[] reinterpret_cast<int8_t*>(_new_arr);
        throw;
    }

    for (size_t i = 0; i != _vec_size; ++i) {
        this->_arr[i].~T();
    }

    delete[] reinterpret_cast<int8_t*>(this->_arr);
    this->_arr = _new_arr;
    this->_vec_capacity = new_capacity;

    return;
}

template <typename T>
void vector<T>::resize(const size_t new_size, const T& val) {
    if (new_size > ULLONG_MAX) throw std::range_error("Capacity overflow");
    if (new_size > _vec_capacity) reserve(new_size);

    if (new_size > _vec_size) {
        for (size_t i = _vec_size; i != new_size; ++i) {
            new (_arr + i) T(val);
        }
    }

    if (new_size < _vec_size) {
        for (size_t i = new_size; i != _vec_size; ++i) {
            _arr[i].~T();
        }
    }

    _vec_size = new_size;
    return;
}

template <typename T>
void vector<T>::shrink_to_fit() {
    if (_vec_size == 0) {
        reserve(1);
        return;
    }

    if (_vec_size < _vec_capacity) {
        T* _new_arr = reinterpret_cast<T*>(new int8_t[sizeof(T) * _vec_size]);

        for (size_t i = 0; i != _vec_size; ++i) {
            new (_new_arr + i) T(_arr);
        }

        delete[] reinterpret_cast<int8_t*>(_new_arr);
        _arr = _new_arr;
        _vec_capacity = _vec_size;
    }
}

template <typename T>
void vector<T>::clear() {
    if (_vec_size == 0) return;

    for (size_t i = 0; i != _vec_size; ++i){
        _arr[i].~T();
    }
    _vec_size = 0;
}

template <typename T>
void vector<T>::push_back(const T& val) {
    if (_vec_capacity == _vec_size) {
        if (_vec_capacity * 2 < _vec_capacity) throw std::range_error("Capacity overflow");

        reserve(2 * _vec_capacity);
    }

    new (_arr + _vec_size) T(val);
    ++_vec_size;
}

template <typename T>
template <typename ...Args>
void vector<T>::emplace_back(const Args&... args) {
    if (_vec_capacity == _vec_size) reserve(2 * _vec_capacity);

    new (_arr + _vec_size) T(args...);
    ++_vec_size;
}

template <typename T>
void vector<T>::pop_back() {
    if (_vec_size == 0) return;

    --_vec_size;
    _arr[_vec_size].~T();
}

template <typename T>
T& vector<T>::operator[](size_t index) {
    return _arr[index];
}

template <typename T>
T& vector<T>::at(size_t index) {
    if (index >= _vec_size) throw std::out_of_range("Index out of range");
    return _arr[index];
}

template <typename T>
T& vector<T>::front() {
    if (_vec_size == 0) throw std::range_error("Vector is empty");
    return _arr[0];
}

template <typename T>
T& vector<T>::back() {
    if (_vec_size == 0) throw std::range_error("Vector is empty");
    return _arr[_vec_size - 1];
}

template <typename T>
const T& vector<T>::operator[](size_t index) const {
    return _arr[index];
}

template <typename T>
const T& vector<T>::at(size_t index) const {
    if (index >= this->_vec_size) throw std::out_of_range("Index out of range");
    return _arr[index];
}

template <typename T>
const T& vector<T>::front() const {
    if (_vec_size == 0) throw std::range_error("Vector is empty");
    return _arr[0];
}

template <typename T>
const T& vector<T>::back() const {
    if (_vec_size == 0) throw std::range_error("Vector is empty");
    return _arr[_vec_size - 1];
}

template <typename T>
size_t vector<T>::size() const {
    return _vec_size;
}

template <typename T>
size_t vector<T>::capacity() const {
    return _vec_capacity;
}

template <typename T>
bool vector<T>::empty() const {
    return (_vec_size == 0);
}

template <typename T>
bool vector<T>::operator==(const vector<T>& other) const {
    if (this->_vec_size != other._vec_size) return false;

    for (size_t i = 0; i != this->_vec_size; ++i) {
        if (this->_arr[i] != other._arr[i]) return false;
    }
    return true;
}

template <typename T>
bool vector<T>::operator!=(const vector<T>& other) const {
    if (this->_vec_size != other._vec_size) return true;

    for (size_t i = 0; i != this->_vec_size; ++i) {
        if (this->_arr[i] != other._arr[i]) return true;
    }
    return false;
}

template <typename T>
vector<T>::~vector() noexcept {
    for (size_t i = 0; i != _vec_size; ++i) {
        _arr[i].~T();
    }
    delete[] reinterpret_cast<int8_t*>(_arr);
    _arr = nullptr;
}