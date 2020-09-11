#pragma once

#include <type_traits>

template <typename T1, typename T2, typename = void>
struct has_not_equal_helper : public std::false_type {};

template <typename T1, typename T2>
struct has_not_equal_helper<T1, T2, std::void_t<decltype(std::declval<T1>() != std::declval<T2>())>>
    : public std::true_type {};

template <typename T1, typename T2, typename = void> struct has_not_equal : public has_not_equal_helper<T1, T2> {};

template <typename T1, typename T2> constexpr bool has_not_equal_v = has_not_equal<T1, T2>::value;