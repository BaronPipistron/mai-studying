#include "../include/MyVector.h"

#include <climits>
#include <memory>

template <typename T>
myVector<T>::myVector(){
    data = reinterpret_cast<T*>(new int8_t[sizeof(T)]);
    vec_size = 0;
    vec_capacity = 1;
}

template <typename T>
myVector<T>::myVector(const myVector<T>& another_vector){
    resize(another_vector.size());

    for (size_t i = 0; i != vec_size; ++i){
        data[i].~T();
        new (data + i) T(another_vector[i]);
    }
}

template <typename T>
myVector<T>::myVector(const std::initializer_list<T>& list){
    T* new_data = reinterpret_cast<T*>(new int8_t[list.size() * sizeof(T)]);

    try{
        std::uninitialized_copy(list.begin(), list.end(), new_data);
    } catch (...){
        delete[] reinterpret_cast<int8_t*>(new_data);
        throw;
    }

    delete[] reinterpret_cast<int8_t*>(data);
    data = new_data;
    vec_size = list.size();
    vec_capacity = list.size();
}

template <typename T>
myVector<T>::~myVector(){
    for (size_t i = 0; i != vec_size; ++i){
        data[i].~T();
    }

    delete[] reinterpret_cast<int8_t*>(data);
}

template <typename T>
void myVector<T>::reserve(size_t reserved_size){
    if (reserved_size > ULLONG_MAX){
        throw std::range_error("Capacity overflow");
    }

    if (reserved_size <= vec_capacity) return;

    T* new_data = reinterpret_cast<T*>(new int8_t[reserved_size * sizeof(T)]);
    try{
        std::uninitialized_copy(data, data + vec_size, new_data);
    } catch(...){
        delete[] reinterpret_cast<int8_t*>(new_data);
        throw;
    }

    for (size_t i = 0; i != vec_size; ++i){
        data[i].~T();
    }
    
    delete[] reinterpret_cast<int8_t*>(data);
    data = new_data;
    vec_capacity = reserved_size;
}

template <typename T>
void myVector<T>::resize(size_t new_size, const T& value){
    if (new_size > ULLONG_MAX){
        throw std::range_error("Capacity overflow");
    }

    if (new_size > vec_capacity) reserve(new_size);

    if (new_size > vec_size){
        for (size_t i = vec_size; i != new_size; ++i){
            new (data + i) T(value);
        }
    }

    if (new_size < vec_size){
        for (size_t i = new_size; i != vec_size; ++i){
            data[i].~T();
        }
    }

    vec_size = new_size;
}

template <typename T>
void myVector<T>::shrink_to_fit(){
    if (vec_size < vec_capacity){
        T* new_data = reinterpret_cast<T*>(new int8_t[vec_size * sizeof(T)]);

        for (size_t i = 0; i != vec_size; ++i){
            new (new_data + i) T(data[i]);
        }

        delete[] reinterpret_cast<int8_t*>(data);
        data = new_data;
        vec_capacity = vec_size;
    }
}

template <typename T>
void myVector<T>::push_back(const T& value){
    if (vec_capacity == vec_size){
        if (vec_capacity * 2 < vec_capacity){
            throw std::range_error("Capacity overflow");
        }

        reserve(2 * vec_capacity);
    }

    new (data + vec_size) T(value);
    ++vec_size;
}

template <typename T>
void myVector<T>::pop_back(){
    if (vec_size == 0) return;

    --vec_size;
    data[vec_size].~T();
}

template <typename T>
T& myVector<T>::at(size_t index){
    if (index >= vec_size){
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

template <typename T>
const T& myVector<T>::front() const{
    if (vec_size == 0){
        throw std::range_error("Vector is empty");
    }

    return data[0];
}

template <typename T>
const T& myVector<T>::back() const{
    if (vec_size == 0){
        throw std::range_error("Vector is empty");
    }

    return data[vec_size - 1];
}

template <typename T>
bool myVector<T>::empty() const{
    return vec_size == 0;
}

template <typename T>
size_t myVector<T>::size() const{
    return vec_size;
}

template <typename T>
size_t myVector<T>::capacity() const{
    return vec_capacity;
}

template <typename T>
void myVector<T>::clear(){
    for (size_t i = 0; i != vec_size; ++i){
        data[i].~T();
    }
    vec_size = 0;
}

template <typename T>
template <typename... Args>
void myVector<T>::emplace_back(const Args& ...args){
    if (vec_capacity == vec_size) reserve(2 * vec_capacity);

    new (data + vec_size) T(args...);
    ++vec_size;
}

template <typename T>
T& myVector<T>::operator[](size_t i) const{
    return data[i];
}

template <typename T>
bool myVector<T>::operator==(const myVector<T>& another_vector){
    if (vec_size != another_vector.vec_size) return false;

    for (size_t i = 0; i != vec_size; ++i){
        if (this->data[i] != another_vector.data[i]) return false;
    }

    return true;
}

template <typename T>
bool myVector<T>::operator!=(const myVector<T>& another_vector){
    if (vec_size != another_vector.vec_size) return true;

    for (size_t i = 0; i != vec_size; ++i){
        if (this->data[i] != another_vector.data[i]) return true;
    }

    return false;
}

template <typename T>
void myVector<T>::erase(size_t element_index){
    if (element_index >= vec_size){
        throw std::out_of_range("Index out of range");
    }

    for (size_t i = element_index; i != vec_size - 1; ++i){
        data[i].~T();
        data[i] = data[i + 1];
    }
    --vec_size;
    resize(vec_size);
}

template <typename T>
void myVector<T>::erase(size_t start, size_t end){
    if (start > end){
        throw std::invalid_argument("Start index must be less than end index");
    }

    if (start > vec_size || end > vec_size){
        throw std::out_of_range("Index out of range");
    }

    size_t to_delet = end - start + 1;
    for (size_t i = start; i != vec_size - to_delet; ++i){
        data[i].~T();
        data[i] = data[i + to_delet];
    }
    vec_size -= to_delet;
    resize(vec_size);
}

template <typename T>
void myVector<T>::swap(myVector<T>& another_vector){
    T* temp_data = data;
    data = another_vector.data;
    another_vector.data = temp_data;

    size_t temp_size = vec_size;
    vec_size = another_vector.vec_size;
    another_vector.vec_size = temp_size;

    size_t temp_capacity = vec_capacity;
    vec_capacity = another_vector.vec_capacity;
    another_vector.vec_capacity = temp_capacity;
}