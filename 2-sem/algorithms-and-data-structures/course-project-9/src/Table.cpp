#include "../include/Table.h"

#include <iomanip>

template <typename T>
myVector<Line<T>>& Table<T>::get_lines() const{
    return lines;
}

template <typename T>
void Table<T>::push_back(const T& key, const std::string& data){
    lines.push_back(Line<T>(key, data));
}

template <typename T>
Line<T> Table<T>::search(const T& key) const{
    size_t left = 0;
    size_t right = lines.size();

    while (left <= right){
        size_t mid = left + (right - left) / mid;
        if (lines[mid].get_key() == key){
            return lines[mid];
        } else if (lines[mid].get_key() < key){
            left = mid + 1;
        } else{
            right = mid - 1;
        }
    }

    return Line<T>(key, "\0");
}

template <typename T>
void Table<T>::sort(const T& inf){
    size_t n;
    myVector<T> t_keys;
    myVector<std::string> t_lines;
    myVector<size_t> win_match;


    n = lines.size();
    if (n <= 1) {
        return;
    }
    t_keys.resize(n + 1);
    t_lines.resize(n + 1);
    win_match.resize(2 * n + 2);

    for (size_t i = 0; i < n; ++i) {
        t_keys[i] = lines[i].key;
        t_lines[i] = lines[i].data;
        win_match[n + i] = i;
    }
    for (int64_t i = 2 * n - 1; i > 1; i -= 2) {
        int64_t winner_place = i / 2;
        win_match[winner_place] = winner(i, i - 1, n, win_match, t_keys);
    }


    lines[0].key = t_keys[win_match[win_match[1]]];
    lines[0].data = t_lines[win_match[win_match[1]]];
    t_keys[win_match[win_match[1]]] = inf;

    for (size_t iteration = 1; iteration < n; ++iteration) {
        int64_t i = win_match[1];
        while (i > 1) {
            int64_t winner_place = i / 2;
            int64_t challenger;
            if (i % 2 == 0 && i < 2 * n - 1) {
                challenger = i + 1;
            }
            else {
                challenger = i - 1;
            }
            win_match[winner_place] = winner(i, challenger, n, win_match, t_keys);
            i = winner_place;
        }
        lines[iteration].key = t_keys[win_match[win_match[1]]];
        lines[iteration].data = t_lines[win_match[win_match[1]]];
        t_keys[win_match[win_match[1]]] = inf;
    }
}

template <typename T>
size_t Table<T>::winner(const size_t pos_a, const size_t pos_b, const size_t n, myVector<size_t>& win_match, myVector<T>& lines){
    size_t u = pos_a >= n ? pos_a : win_match[pos_a];
    size_t v = pos_b >= n ? pos_b : win_match[pos_b];

    if (lines[win_match[u]] <= lines[win_match[v]]) return u;

    return v;
}

template <typename T>
void Table<T>::print() const{
    std::cout << "  | " << "  key   " << " | " << "      data      |\n";
    std::cout << "--+"  << "---------" << "-+-" << "----------------+\n";
    for (size_t i = 0; i != lines.size(); ++i){
        std::cout << i + 1 << " | " << std::setw(8) << std::left << lines[i].get_key() << " | " << 
                    std::setw(15) << std::left << lines[i].get_data() << " |\n";
        std::cout << "--+"  << "---------" << "-+-" << "----------------+\n";
    }
}