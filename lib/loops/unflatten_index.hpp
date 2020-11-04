#pragma once

#include <array>
#include <numeric> //std::accumulate
#include <algorithm> //std::transform
#include "loops/storage_order.hpp"
#include "loops/flatten_index.hpp"
#include "utils/runtime_assert.hpp"
#include "utils/can_throw.hpp"

namespace JADA {



template <size_t N, StorageOrder storage>
constexpr std::array<idx_t, N> unravel(std::array<idx_t, N> dims, idx_t idx) {

    Utils::runtime_assert(std::accumulate(std::begin(dims), std::end(dims), idx_t(1), std::multiplies<idx_t>{}) 
                          > idx, "Index out of bounds"
    
    );

    
    
    std::array<idx_t, N> md_idx;
    auto mult = get_multipliers<N, storage>(dims);


    if constexpr (storage == StorageOrder::RowMajor){

        for (idx_t i = 0; i < N; ++i){
            md_idx[i] = idx / mult[i];
            idx -= md_idx[i]*mult[i];
        } 
    }

    else {

        for (idx_t i=N-1; int(i) >= 0; --i){
            md_idx[i] = idx / mult[i];
            idx -= md_idx[i]*mult[i];
        }
    }


    
    return md_idx;

}




template <typename INT>
constexpr std::array<INT, 1> unflatten(const std::array<INT, 1>& dimension,
                                       INT idx) noexcept(Utils::can_throw) {

#ifdef DEBUG
    INT total = std::accumulate(dimension.begin(), dimension.end(), INT(1), std::multiplies<INT>{});
    Utils::runtime_assert(idx < total, "Index unflatten out of bounds.");
#endif

    return {idx};
}

template <typename INT>
constexpr std::array<INT, 2> unflatten(const std::array<INT, 2>& dimension,
                                       INT idx) noexcept(Utils::can_throw) {

#ifdef DEBUG
    INT total = std::accumulate(dimension.begin(), dimension.end(), INT(1), std::multiplies<INT>{});
    Utils::runtime_assert(idx < total, "Index unflatten out of bounds.");
#endif

    INT j = idx / dimension[1];
    INT i = idx % dimension[1];

    return {j, i};
}

template <typename INT>
constexpr std::array<INT, 3> unflatten(const std::array<INT, 3>& dimension,
                                       INT idx) noexcept(Utils::can_throw) {

#ifdef DEBUG
    INT total = std::accumulate(dimension.begin(), dimension.end(), INT(1), std::multiplies<INT>{});
    Utils::runtime_assert(idx < total, "Index unflatten out of bounds.");
#endif

    INT k    = idx / (dimension[1] * dimension[2]);
    INT temp = idx - (k * dimension[1] * dimension[2]);
    INT j    = temp / dimension[2];
    INT i    = temp % dimension[2];
    
    return {k, j, i};


}

} // namespace JADA