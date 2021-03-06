#pragma once

#include <type_traits>


///////////////////////////////////////////////////////////////////////////////////
template <typename T, typename = void> struct is_scalar : std::false_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_scalar<T, std::enable_if_t<std::is_arithmetic_v<T>>> : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T> constexpr bool is_scalar_v = is_scalar<T>::value;

///////////////////////////////////////////////////////////////////////////////////

