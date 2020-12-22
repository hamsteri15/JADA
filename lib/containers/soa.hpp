#pragma once
#include <tuple>
#include <vector>

namespace JADA{


namespace detail{

template<typename... Ts>
struct Typelist{
};

// Declare List
template<class> class VectorTypeList;

// Specialize it, in order to drill down into the template parameters.
template<template<typename...Args> class t, typename ...Ts>
struct VectorTypeList<t<Ts...>> {
    using type = std::tuple<std::vector<Ts>...>;
};


}


template<class... Elements>
struct Soa
{

    //static constexpr size_t n_elements = sizeof(Elements...);

};






}