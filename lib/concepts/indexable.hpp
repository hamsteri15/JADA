#pragma once

#include <concepts>

namespace JADA{


template <typename T> concept Indexable = requires(T t) {
    
    {t[size_t(0)]} -> auto;
    //{std::end(range)} -> auto;
    
};



}