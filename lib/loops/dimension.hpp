#pragma once
#include <array>
#include <cstddef>

namespace JADA{




template<size_t N>
struct dimension{

    std::array<size_t, N> storage;

    size_t& operator[](size_t i) {return storage[i];}
    size_t operator[](size_t i) const {return storage[i];}


    auto begin() {return storage.begin();}
    auto begin() const {return storage.begin();}
    auto cbegin() const {return storage.cbegin();}

    auto end() {return storage.end();}
    auto end() const {return storage.end();}
    auto cend() {return storage.cend();}

};


//template <size_t N> using position = std::array<idx_t, N>;


}
