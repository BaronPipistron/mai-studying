#include "../include/Line.h"

template <typename T>
Line<T>::Line(const T& key, const std::string& data): key(key), data(data){}

template <typename T>
T Line<T>::get_key(){
    return key;
}

template <typename T>
std::string Line<T>::get_data() const{
    return data;
}

template <typename T>
bool Line<T>::operator<(const Line<T>& other) const{
    return key < other.key;
}