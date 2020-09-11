#pragma once

#include <type_traits>


template <typename T1, typename T2, typename = void>
struct has_add_assign_helper : public std::false_type {};

template <typename T1, typename T2>
struct has_add_assign_helper<T1, T2, decltype(std::declval<T1>() += std::declval<T2>(), void())>
    : public std::true_type {};


template <typename T1, typename T2, typename = void> struct has_add_assign : public has_add_assign_helper<T1, T2> {};

template <typename T1, typename T2> constexpr bool has_add_assign_v = has_add_assign<T1, T2>::value;

