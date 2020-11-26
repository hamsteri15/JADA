#pragma once
#include <array>
#include <cstddef>

namespace JADA{




template<size_t N>
struct dimension{

    std::array<size_t, N> storage;

    //TODO: add a concept for the integer, currently this is ok ass storage is indexed
    template<typename INT> 
    size_t operator[](INT i) {return storage[i];}

    template<typename INT> 
    size_t operator[](INT i) const {return storage[i];}


    auto begin() {return storage.begin();}
    auto begin() const {return storage.begin();}
    auto cbegin() const {return storage.cbegin();}

    auto end() {return storage.end();}
    auto end() const {return storage.end();}
    auto cend() {return storage.cend();}

};


//template <size_t N> using position = std::array<idx_t, N>;


}
