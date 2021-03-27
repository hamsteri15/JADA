#pragma once

#include "utils/math_vector.hpp"
#include "loops/dimension.hpp"
#include "loops/index_type.hpp"
#include <array>

namespace JADA {

template <size_t L>
struct position : public Utils::MathVectorBase<idx_t, L, position<L>> {

    using element_t = idx_t;

    inline constexpr position() = default;

    inline constexpr position(std::array<element_t, L> arr) : m_storage(arr) {} 

    inline constexpr position(dimension<L> dim) {
        for (size_t i = 0; i < L; ++i) {
            m_storage[i] = static_cast<idx_t>(dim[i]);
        }
    } 


    constexpr position(position const&) noexcept = default;
    constexpr position(position&&) noexcept = default;
    constexpr position& operator=(position const&) noexcept = default;
    constexpr position& operator=(position&&) noexcept = default;


    constexpr position(std::initializer_list<element_t> list) {

        if (list.size() > L) {
            throw std::logic_error("Invalid paramenter count for vector");
        }
        std::move(list.begin(), list.end(), m_storage.begin());

    }

    bool all_positive() const {
        for (auto e : m_storage){
            if (e < 0) {
                return false;
            }
        }
        return true;
    }

    constexpr const element_t* get_ptr() const { return m_storage.data(); }
    constexpr element_t* get_ptr() { return m_storage.data(); }

private:
    std::array<element_t, L> m_storage;
};



template <size_t N>
auto operator<(const position<N>& lhs, const position<N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] >= rhs[i]) { return false; }
    }
    return true;
}

template <size_t N>
auto operator<=(const position<N>& lhs, const position<N>& rhs) {
    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] > rhs[i]) { return false; }
    }
    return true;
}

template <size_t N>
auto operator>(const position<N>& lhs, const position<N>& rhs) {
    return !(lhs <= rhs);
}

template <size_t N>
auto operator>=(const position<N>& lhs, const position<N>& rhs) {
    return !(lhs < rhs);
}




//================================
// <=> Comparisons with dimension
//================================

template <size_t N>
auto operator==(const position<N>& lhs, const dimension<N>& rhs) {
    return (lhs == position<N>(rhs));
}

template <size_t N>
auto operator!=(const position<N>& lhs, const dimension<N>& rhs) {
    return !(lhs == rhs);
}

template <size_t N>
auto operator<(const position<N>& lhs, const dimension<N>& rhs) {
    return (lhs < position<N>(rhs));

}

template <size_t N>
auto operator<=(const position<N>& lhs, const dimension<N>& rhs) {
    return (lhs <= position<N>(rhs));
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
