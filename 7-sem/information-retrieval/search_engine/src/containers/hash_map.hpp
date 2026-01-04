#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <utility>

namespace ir {

inline uint64_t fnv1a_64(const std::string& s) {
    uint64_t h = 1469598103934665603ull;

    for (unsigned char c : s) {
        h ^= static_cast<uint64_t>(c);
        h *= 1099511628211ull;
    }

    return h;
}

template<typename K>
struct DefaultHash;

template<>
struct DefaultHash<std::string> {
    uint64_t operator()(const std::string& s) const { return fnv1a_64(s); }
};

template<>
struct DefaultHash<uint32_t> {
    uint64_t operator()(uint32_t v) const {
        return static_cast<uint64_t>(v) * 11400714819323198485ull;
    }
};

template<typename K>
struct DefaultEq {
    bool operator()(const K& a, const K& b) const { return a == b; }
};

template<typename K, typename V, typename Hash = DefaultHash<K>, typename Eq = DefaultEq<K>>
class HashMap {
public:
    HashMap() { rehash(16); }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // Returns pointer to value if found; nullptr otherwise
    V* find_ptr(const K& key) {
        size_t idx = find_slot(key);
        if (idx == npos_ || table_[idx].state != State::OCCUPIED) return nullptr;
        
        return &table_[idx].value;
    }

    const V* find_ptr(const K& key) const {
        size_t idx = find_slot(key);
        if (idx == npos_ || table_[idx].state != State::OCCUPIED) return nullptr;

        return &table_[idx].value;
    }

    bool contains(const K& key) const { return find_ptr(key) != nullptr; }

    // Inserts or assigns
    void put(const K& key, const V& value) {
        ensure_capacity_for_insert();
        insert_or_assign(key, value);
    }

    // Inserts default value if absent and returns reference
    V& operator[](const K& key) {
        ensure_capacity_for_insert();
        size_t idx = probe_for_insert(key);

        if (table_[idx].state == State::OCCUPIED) return table_[idx].value;

        table_[idx].key = key;
        table_[idx].value = V{};
        table_[idx].state = State::OCCUPIED;
        ++size_;

        return table_[idx].value;
    }

    // Removes key, returns true if removed
    bool erase(const K& key) {
        size_t idx = find_slot(key);

        if (idx == npos_ || table_[idx].state != State::OCCUPIED) return false;
        
        table_[idx].state = State::DELETED;
        --size_;
        ++deleted_;

        return true;
    }

    // Iterate over occupied entries
    template<typename F>
    void for_each(F&& f) const {
        for (const auto& e : table_) {
            if (e.state == State::OCCUPIED) f(e.key, e.value);
        }
    }

private:
    enum class State : uint8_t { EMPTY = 0, OCCUPIED = 1, DELETED = 2 };

    struct Entry {
        K key{};
        V value{};
        State state {State::EMPTY};
    };

    static constexpr size_t npos_ = static_cast<size_t>(-1);

    std::vector<Entry> table_;
    size_t size_{0};
    size_t deleted_{0};
    Hash hasher_{};
    Eq eq_{};

    double load_factor() const {
        if (table_.empty()) return 1.0;
        return static_cast<double>(size_ + deleted_) / static_cast<double>(table_.size());
    }

    void ensure_capacity_for_insert() {
        if (load_factor() >= 0.70) rehash(table_.size() * 2);
    }

    size_t find_slot(const K& key) const {
        if (table_.empty()) return npos_;

        const size_t cap = table_.size();
        size_t idx = static_cast<size_t>(hasher_(key) % cap);

        for (size_t i = 0; i < cap; ++i) {
            const auto& e = table_[idx];

            if (e.state == State::EMPTY) return npos_;
            if (e.state == State::OCCUPIED && eq_(e.key, key)) return idx;

            idx = (idx + 1) % cap;
        }

        return npos_;
    }

    size_t probe_for_insert(const K& key) {
        const size_t cap = table_.size();
        size_t idx = static_cast<size_t>(hasher_(key) % cap);
        size_t first_deleted = npos_;

        for (size_t i = 0; i < cap; ++i) {
            auto& e = table_[idx];

            if (e.state == State::OCCUPIED) {
                if (eq_(e.key, key)) return idx;
            } else if (e.state == State::DELETED) {
                if (first_deleted == npos_) first_deleted = idx;
            } else { // EMPTY
                return (first_deleted != npos_) ? first_deleted : idx;
            }

            idx = (idx + 1) % cap;
        }

        return (first_deleted != npos_) ? first_deleted : 0;
    }

    void insert_or_assign(const K& key, const V& value) {
        size_t idx = probe_for_insert(key);
        auto& e = table_[idx];

        if (e.state == State::OCCUPIED) {
            e.value = value;
            return;
        }

        e.key = key;
        e.value = value;
        e.state = State::OCCUPIED;
        ++size_;
    }

    void rehash(size_t new_cap) {
        if (new_cap < 16) new_cap = 16;

        std::vector<Entry> old = std::move(table_);
        table_.assign(new_cap, Entry{});
        size_ = 0;
        deleted_ = 0;
        
        for (const auto& e : old) {
            if (e.state == State::OCCUPIED) {
                size_t idx = probe_for_insert(e.key);
                table_[idx].key = e.key;
                table_[idx].value = e.value;
                table_[idx].state = State::OCCUPIED;
                ++size_;
            }
        }
    }
};

} // namespace ir
