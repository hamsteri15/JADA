#pragma once


#include <array>
#include <cstddef>

#include "utils/math_vector_base.hpp"



namespace JADA{

template <size_t L>
struct dimension : public Utils::MathVectorBase<size_t, L, dimension<L>> {

    using element_t = size_t;

    inline constexpr dimension() = default;
    constexpr dimension(dimension const&) noexcept = default;
    constexpr dimension(dimension&&) noexcept = default;
    constexpr dimension& operator=(dimension const&) noexcept = default;
    constexpr dimension& operator=(dimension&&) noexcept = default;

    constexpr dimension(std::initializer_list<element_t> list) {

        if (list.size() > L) {
            throw std::logic_error("Invalid paramenter count for vector");
        }
        std::move(list.begin(), list.end(), m_storage.begin());

        Utils::runtime_assert(this->min() >= 0, "Negative position.");
    }

    const element_t* get_ptr() const { return m_storage.data(); }
    element_t* get_ptr() { return m_storage.data(); }





private:
    std::array<element_t, L> m_storage;
};



}


// These have to be outside the JADA namespace for conversion to structured
// bindings
template <size_t N> struct std::tuple_size<JADA::dimension<N>> {
    static const std::size_t value = N;
};

template <std::size_t I, size_t N>
struct std::tuple_element<I, JADA::dimension<N>> {
    using type = size_t;
};