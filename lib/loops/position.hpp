#pragma once

#include "loops/dimension.hpp"
#include "loops/index_type.hpp"
#include <array>

namespace JADA {

// template <size_t N> using position = std::array<idx_t, N>;

template <size_t N> struct position {

    using element_t = idx_t;

    std::array<element_t, N> storage;

    template <size_t I> element_t get() { return storage[I]; }

    element_t& operator[](size_t i) { return storage[i]; }
    element_t  operator[](size_t i) const { return storage[i]; }

    auto begin() { return storage.begin(); }
    auto begin() const { return storage.begin(); }
    auto cbegin() const { return storage.cbegin(); }

    auto end() { return storage.end(); }
    auto end() const { return storage.end(); }
    auto cend() { return storage.cend(); }

    auto operator<=>(const position<N>& rhs) const = default;

    //Math ops
    auto& operator+=(const position<N>& rhs) {
        for (size_t i = 0; i < N; ++i) { storage[i] += rhs[i]; }
        return *this; 
    }

    auto& operator-=(const position<N>& rhs) {
        for (size_t i = 0; i < N; ++i) { storage[i] -= rhs[i]; }
        return *this; 
    }

    auto& operator*=(const position<N>& rhs) {
        for (size_t i = 0; i < N; ++i) { storage[i] *= rhs[i]; }
        return *this; 
    }

    auto& operator/=(const position<N>& rhs) {
        for (size_t i = 0; i < N; ++i) { storage[i] /= rhs[i]; }
        return *this; 
    }


    friend position<N> operator+(position<N> lhs, const position<N>& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend position<N> operator-(position<N> lhs, const position<N>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend position<N> operator*(position<N> lhs, const position<N>& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend position<N> operator/(position<N> lhs, const position<N>& rhs) {
        lhs /= rhs;
        return lhs;
    }
};


//================================
// <=> Comparisons with dimension
//================================

template <size_t N>
auto operator==(const position<N>& lhs, const dimension<N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] != idx_t(rhs[i])) { return false; }
    }
    return true;
}

template <size_t N>
auto operator!=(const position<N>& lhs, const dimension<N>& rhs) {
    return !(lhs == rhs);
}

template <size_t N>
auto operator<(const position<N>& lhs, const dimension<N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] >= idx_t(rhs[i])) { return false; }
    }
    return true;
}

template <size_t N>
auto operator<=(const position<N>& lhs, const dimension<N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] > idx_t(rhs[i])) { return false; }
    }
    return true;
}

template <size_t N>
auto operator>(const position<N>& lhs, const dimension<N>& rhs) {
    return !(lhs <= rhs);
}

template <size_t N>
auto operator>=(const position<N>& lhs, const dimension<N>& rhs) {
    return !(lhs < rhs);
}

} // namespace JADA

// These have to be outside the JADA namespace for conversion to structured
// bindings
template <size_t N> struct std::tuple_size<JADA::position<N>> {
    static const std::size_t value = N;
};

template <std::size_t I, size_t N>
struct std::tuple_element<I, JADA::position<N>> {
    using type = JADA::idx_t;
};
