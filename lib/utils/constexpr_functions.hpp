#pragma once

#include <type_traits>


//These functions are defined because no constexpr equivalents are in the standard yet.


namespace JADA::Utils {


template <class T, std::enable_if_t<std::is_arithmetic_v<T>>...>
constexpr auto constexpr_abs(T const& x) noexcept {
    return x < 0 ? -x : x;
}

template <typename T> constexpr T const& constexpr_max(T const& a, T const& b) {
    return a > b ? a : b;
}

} // namespace JADA::Utils
