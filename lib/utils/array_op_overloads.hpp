#pragma once
#include <array>

/*
Handy operator overloads for scalar arrays.

Operations:
array + - * / % array
array + - * / % scalar
scalar + - * / % array
array == != > < >= <= array
array == != > < >= <= scalar
scalar == != > < >= <= array


*/

namespace JADA::Utils {

namespace ArrayOpOverloads {

namespace detail {

template <typename T, std::size_t... Is>
constexpr std::array<T, sizeof...(Is)> make_array(const T& value, std::index_sequence<Is...>) {
    return {{(static_cast<void>(Is), value)...}};
}
} // namespace detail

template <std::size_t N, typename T> constexpr std::array<T, N> make_array(const T& value) {
    return detail::make_array(value, std::make_index_sequence<N>());
}

// negate
template <class T, size_t N, class = std::enable_if_t<std::is_arithmetic<T>::value>>
constexpr std::array<T, N> operator-(const std::array<T, N>& a) {

    std::array<T, N> ret{T()};
    for (size_t i = 0; i < N; ++i) { ret[i] = -a[i]; }
    return ret;
}

template <class ResultType, class Arr1, class Arr2, std::size_t... Is>
constexpr ResultType AddImpl(const Arr1& a, const Arr2& b, std::index_sequence<Is...>) {
    return {a[Is] + b[Is]...};
}

template <class ResultType, class Arr1, class Arr2, std::size_t... Is>
constexpr ResultType MinusImpl(const Arr1& a, const Arr2& b, std::index_sequence<Is...>) {
    return {a[Is] - b[Is]...};
}

template <class ResultType, class Arr1, class Arr2, std::size_t... Is>
constexpr ResultType MultImpl(const Arr1& a, const Arr2& b, std::index_sequence<Is...>) {
    return {a[Is] * b[Is]...};
}

template <class ResultType, class Arr1, class Arr2, std::size_t... Is>
constexpr ResultType DivImpl(const Arr1& a, const Arr2& b, std::index_sequence<Is...>) {
    return {a[Is] / b[Is]...};
}

// modulo
template <class ResultType, class Arr1, class Arr2, std::size_t... Is>
constexpr ResultType ModImpl(const Arr1& a, const Arr2& b, std::index_sequence<Is...>) {
    return {a[Is] % b[Is]...};
}

// array + array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() + std::declval<R>())>
constexpr std::array<Ret, N> operator+(const std::array<L, N>& lhs, const std::array<R, N>& rhs) {
    return AddImpl<std::array<Ret, N>>(lhs, rhs, std::make_index_sequence<N>());
}

// array - array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() - std::declval<R>())>
constexpr std::array<Ret, N> operator-(const std::array<L, N>& lhs, const std::array<R, N>& rhs) {
    return MinusImpl<std::array<Ret, N>>(lhs, rhs, std::make_index_sequence<N>());
}

// array * array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() * std::declval<R>())>
constexpr std::array<Ret, N> operator*(const std::array<L, N>& lhs, const std::array<R, N>& rhs) {
    return MultImpl<std::array<Ret, N>>(lhs, rhs, std::make_index_sequence<N>());
}

// array / array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() / std::declval<R>())>
constexpr std::array<Ret, N> operator/(const std::array<L, N>& lhs, const std::array<R, N>& rhs) {
    return DivImpl<std::array<Ret, N>>(lhs, rhs, std::make_index_sequence<N>());
}

// array % array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() % std::declval<R>())>
constexpr std::array<Ret, N> operator%(const std::array<L, N>& lhs, const std::array<R, N>& rhs) {
    return ModImpl<std::array<Ret, N>>(lhs, rhs, std::make_index_sequence<N>());
}

// array + scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() + std::declval<R>())>
constexpr std::array<Ret, N> operator+(const std::array<L, N>& lhs, const R& rhs) {

    return AddImpl<std::array<Ret, N>>(lhs, make_array<N, R>(rhs), std::make_index_sequence<N>());
}

// array - scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() - std::declval<R>())>
constexpr std::array<Ret, N> operator-(const std::array<L, N>& lhs, const R& rhs) {

    return MinusImpl<std::array<Ret, N>>(lhs, make_array<N, R>(rhs), std::make_index_sequence<N>());
}

// array * scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() * std::declval<R>())>
constexpr std::array<Ret, N> operator*(const std::array<L, N>& lhs, const R& rhs) {

    return MultImpl<std::array<Ret, N>>(lhs, make_array<N, R>(rhs), std::make_index_sequence<N>());
}

// array / scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() / std::declval<R>())>
constexpr std::array<Ret, N> operator/(const std::array<L, N>& lhs, const R& rhs) {

    return DivImpl<std::array<Ret, N>>(lhs, make_array<N, R>(rhs), std::make_index_sequence<N>());
}

// array % scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() % std::declval<R>())>
constexpr std::array<Ret, N> operator%(const std::array<L, N>& lhs, const R& rhs) {

    return ModImpl<std::array<Ret, N>>(lhs, make_array<N, R>(rhs), std::make_index_sequence<N>());
}

// scalar + array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() + std::declval<R>())>
constexpr std::array<Ret, N> operator+(const L& lhs, const std::array<R, N>& rhs) {

    return AddImpl<std::array<Ret, N>>(make_array<N, L>(lhs), rhs, std::make_index_sequence<N>());
}

// scalar - array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() - std::declval<R>())>
constexpr std::array<Ret, N> operator-(const L& lhs, const std::array<R, N>& rhs) {

    return MinusImpl<std::array<Ret, N>>(make_array<N, L>(lhs), rhs, std::make_index_sequence<N>());
}

// scalar * array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() * std::declval<R>())>
constexpr std::array<Ret, N> operator*(const L& lhs, const std::array<R, N>& rhs) {

    return MultImpl<std::array<Ret, N>>(make_array<N, L>(lhs), rhs, std::make_index_sequence<N>());
}

// scalar / array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() / std::declval<R>())>
constexpr std::array<Ret, N> operator/(const L& lhs, const std::array<R, N>& rhs) {

    return DivImpl<std::array<Ret, N>>(make_array<N, L>(lhs), rhs, std::make_index_sequence<N>());
}

// scalar % array
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>,
          class Ret = decltype(std::declval<L>() % std::declval<R>())>
constexpr std::array<Ret, N> operator%(const L& lhs, const std::array<R, N>& rhs) {

    return ModImpl<std::array<Ret, N>>(make_array<N, L>(lhs), rhs, std::make_index_sequence<N>());
}

//////////////////////////////////
// Comparisons
/////////////////////////////////

// array == scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>>
constexpr bool operator==(const std::array<L, N>& lhs, const R& rhs) {

    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] != rhs) { return false; }
    }
    return true;
}

// array != scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>>
constexpr bool operator!=(const std::array<L, N>& lhs, const R& rhs) {

    return !(lhs == rhs);
}

// array <= scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>>
constexpr bool operator<=(const std::array<L, N>& lhs, const R& rhs) {

    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] > rhs) { return false; }
    }
    return true;
}

// array >= scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>>
constexpr bool operator>=(const std::array<L, N>& lhs, const R& rhs) {

    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] < rhs) { return false; }
    }
    return true;
}

// array < scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>>
constexpr bool operator<(const std::array<L, N>& lhs, const R& rhs) {

    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] > rhs) { return false; }
    }
    return true;
}

// array > scalar
template <class L,
          class R,
          size_t N,
          class = std::enable_if_t<std::is_arithmetic<L>::value && std::is_arithmetic<R>::value>>
constexpr bool operator>(const std::array<L, N>& lhs, const R& rhs) {

    for (size_t i = 0; i < N; ++i) {
        if (lhs[i] < rhs) { return false; }
    }
    return true;
}

} // namespace ArrayOpOverloads

} // namespace Utils