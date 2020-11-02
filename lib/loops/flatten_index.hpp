#pragma once

#include <array>
#include <numeric> //std::accumulate
#include <tuple>
#include "loops/storage_order.hpp"
#include "utils/runtime_assert.hpp"
#include "utils/can_throw.hpp"
#include "loops/index_type.hpp"
#include "loops/position.hpp"

namespace JADA {


template<size_t N, idx_t I, StorageOrder storage>
constexpr idx_t get_shift(std::array<idx_t, N> dimension){

    static_assert(I < N, "Shift index out of bounds");

    if constexpr (storage == StorageOrder::RowMajor){


        return std::accumulate(std::begin(dimension) + int(I)+1, 
                               std::end(dimension), 
                               idx_t(1), 
                               std::multiplies<idx_t>{});

    }

    return std::accumulate(std::begin(dimension), 
                            std::begin(dimension) + int(I), 
                            idx_t(1), 
                            std::multiplies<idx_t>{});

}

template< size_t N, StorageOrder storage>
constexpr std::array<idx_t, N> get_multipliers(std::array<idx_t, N> arr)
{
    return [&]<auto... Is>(std::index_sequence<Is...>) { 
        return std::array<idx_t, N>{ get_shift<N, Is, storage>(arr)...};
    }(std::make_integer_sequence <idx_t, N>{});
}

template<size_t N, StorageOrder storage>
constexpr idx_t flatten(std::array<idx_t, N> dimension, position<N> idx) noexcept(Utils::can_throw){

    Utils::runtime_assert(idx < dimension, "Dimensions out of bounds");

    const auto mult = get_multipliers<N, storage>(dimension);

    idx_t index = 0;
    for (idx_t i = 0; i < N; ++i){
        index += idx[i] * mult[i];
    }
    return index;

}

template<StorageOrder storage, class ...Is>
constexpr idx_t flatten(std::array<idx_t, sizeof...(Is)> dimension, Is... indices){

    return flatten<sizeof...(Is), storage>(
        dimension, std::to_array<idx_t>({idx_t(indices)...})
    );

}

/*

template< class INT>
constexpr INT flatten(const std::array<INT, 1>& dimension,
                         INT                       i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(i < dimension[0], "Index 0 Out of bounds");

    return i;
}

template< class INT>
constexpr INT
flatten(const std::array<INT, 2>& dimension, INT j, INT i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(j < dimension[0], "Index 0 Out of bounds");
    Utils::runtime_assert(i < dimension[1], "Index 1 Out of bounds");

    return i + dimension[1] * j;
}

template< class INT>
constexpr INT flatten(const std::array<INT, 3>& dimension,
                         INT                       k,
                         INT                       j,
                         INT                       i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(k < dimension[0], "Index 0 Out of bounds");
    Utils::runtime_assert(j < dimension[1], "Index 1 Out of bounds");
    Utils::runtime_assert(i < dimension[2], "Index 2 Out of bounds");

    return i + j * dimension[2] + k * dimension[1] * dimension[2];
}

template< class INT>
constexpr INT flatten(const std::array<INT, 4>& dimension,
                         INT                       l,
                         INT                       k,
                         INT                       j,
                         INT                       i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(l < dimension[0], "Index 0 Out of bounds");
    Utils::runtime_assert(k < dimension[1], "Index 1 Out of bounds");
    Utils::runtime_assert(j < dimension[2], "Index 2 Out of bounds");
    Utils::runtime_assert(i < dimension[3], "Index 3 Out of bounds");

    return i + j * dimension[3] + k * dimension[3] * dimension[2] +
           l * dimension[3] * dimension[2] * dimension[1];
}

*/


} // namespace Math
