#pragma once

#include <type_traits>

#include "communication/jada_type_to_mpi.hpp"



template <typename T, typename = void>
struct is_mpi_convertible_helper : public std::false_type {};

template <typename T>
struct is_mpi_convertible_helper<T, std::void_t<decltype(Communication::MpiDatatype<T>())>>
    : public std::true_type {};



template <typename T, typename = void> struct is_mpi_convertible : public is_mpi_convertible_helper<T> {};

template <typename T> constexpr bool is_mpi_convertible_v = is_mpi_convertible<T>::value;


