#pragma once
#include <array>
#include "loops/index_type.hpp"

namespace JADA{

//template <size_t N> using position = std::array<idx_t, N>;

template<size_t N>
struct position{

    std::array<idx_t, N> storage;

    template<size_t I>
    idx_t get() {return storage[I];}

    auto& operator[](size_t i) {return storage[i];}

    auto operator[](size_t i) const {return storage[i];}


    auto begin() {return storage.begin();}
    auto begin() const {return storage.begin();}
    auto cbegin() const {return storage.cbegin();}

    auto end() {return storage.end();}
    auto end() const {return storage.end();}
    auto cend() {return storage.cend();}


    auto operator<=>(const position<N>& rhs) const = default;


};

}


//These have to be outside the JADA namespace for conversion to structured bindings
template<size_t N>
struct std::tuple_size<JADA::position<N> >
{ static const std::size_t value = N; };


template<std::size_t I, size_t N> 
struct std::tuple_element<I, JADA::position<N>> { using type = JADA::idx_t; };
