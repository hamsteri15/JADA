#pragma once

#include <type_traits>

#include "type_traits/has_equal.hpp"
#include "type_traits/has_not_equal.hpp"





// has_comparison_ops satisfies:
// type == type
// type != type


///////////////////////////////////////////////////////////////////////////////////

template <typename T, typename = void>
struct has_comparison_ops : std::false_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct has_comparison_ops<T, 
        std::enable_if_t< 
           has_equal_v<T,T>
        && has_not_equal_v<T,T>
        >>
    : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////////


template <typename T>
constexpr bool has_comparison_ops_v = has_comparison_ops<T>::value;

