#pragma once

#include <type_traits>

#include "type_traits/has_add.hpp"
#include "type_traits/has_subtract.hpp"
#include "type_traits/has_multiply.hpp"
#include "type_traits/has_divide.hpp"

#include "type_traits/has_add_assign.hpp"
#include "type_traits/has_subtract_assign.hpp"
#include "type_traits/has_multiply_assign.hpp"
#include "type_traits/has_divide_assign.hpp"



// has_arithmetic_ops satisfies:
// type + - * / type
// scalar + - * / type
// type + - * / scalar
// type += -= *= /= type
// type += -= *= /= scalar

///////////////////////////////////////////////////////////////////////////////////

template <typename T, typename = void>
struct has_arithmetic_ops : std::false_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct has_arithmetic_ops<T, 
        std::enable_if_t< 
           has_add_v<T,T>
        && has_subtract_v<T,T> 
        && has_multiply_v<T,T>
        && has_divide_v<T,T> 
        //TODO: Make double some arithmetric type
        && has_add_v<double,T>
        && has_subtract_v<double,T> 
        && has_multiply_v<double,T>
        && has_divide_v<double,T> 
        //
        && has_add_v<T, double>
        && has_subtract_v<T, double> 
        && has_multiply_v<T, double>
        && has_divide_v<T, double>
        //
        && has_add_assign_v<T&,T>
        && has_subtract_assign_v<T&,T>
        && has_multiply_assign_v<T&,T>
        && has_divide_assign_v<T&,T>
        //
        && has_add_assign_v<T&,double>
        && has_subtract_assign_v<T&,double>
        && has_multiply_assign_v<T&,double>
        && has_divide_assign_v<T&,double>
        >>
    : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////////


template <typename T>
constexpr bool has_arithmetic_ops_v = has_arithmetic_ops<T>::value;

