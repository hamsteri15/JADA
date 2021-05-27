#pragma once

#include <concepts>

#include "concepts/iterator.hpp"

namespace JADA {


template <typename T> using Iterator_type = decltype(std::begin(std::declval<T>()));


template <typename T> concept Iterable = requires(T range) {
    
    {std::begin(range)} -> auto;
    {std::end(range)} -> auto;
    //requires Iterator<Iterator_type<T>>; //TODO: Check that the return type of begin is an iterator
};

} // namespace JADA