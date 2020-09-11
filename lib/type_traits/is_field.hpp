#pragma once

#include <type_traits>


///////////////////////////////////////////////////////////////////////////////////
template <typename T, typename = void> struct is_field : std::false_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T> struct is_field<T, std::enable_if_t<T::is_field>> : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T> constexpr bool is_field_v = is_field<T>::value;

