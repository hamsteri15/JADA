#pragma once

#include <type_traits>

template <typename T1, typename T2, typename = void>
struct has_divide_helper : public std::false_type {};

template <typename T1, typename T2>
struct has_divide_helper<T1, T2, std::void_t<decltype(std::declval<T1>() / std::declval<T2>())>>
    : public std::true_type {};

template <typename T1, typename T2, typename = void> struct has_divide : public has_divide_helper<T1, T2> {};

template <typename T1, typename T2> constexpr bool has_divide_v = has_divide<T1, T2>::value;
