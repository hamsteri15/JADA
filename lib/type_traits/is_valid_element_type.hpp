#pragma once

#include <type_traits>

#include "type_traits/has_arithmetic_ops.hpp"
#include "type_traits/has_comparison_ops.hpp"
#include "type_traits/is_h5_convertible.hpp"
#include "type_traits/is_mpi_convertible.hpp"
#include "type_traits/is_field.hpp"
// A jada 'type' must satisfy
// is default constructible, not pointer etc.
// type + - * / type
// scalar + - * / type
// type + - * / scalar
// comparison operators type ==, != type
// conversion to H5Datatype
// conversion to MpiDatatype<type>


///////////////////////////////////////////////////////////////////////////////////

template <typename T, typename = void>
struct is_valid_element_type : std::false_type {};

///////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_valid_element_type<T, 
        std::enable_if_t< 
        std::is_default_constructible_v<T>
        &&   has_arithmetic_ops_v<T>
        && has_comparison_ops_v<T>
        && is_h5_convertible_v<T>
        && is_mpi_convertible_v<T>
        && !is_field_v<T> 
        >>
    : public std::true_type {};

///////////////////////////////////////////////////////////////////////////////////


template <typename T>
constexpr bool is_valid_element_type_v = is_valid_element_type<T>::value;

