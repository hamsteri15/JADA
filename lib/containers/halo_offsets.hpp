#pragma once

#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/direction.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{

template<size_t N>
static constexpr dimension<N> halo_dims(dimension<N> dims, dimension<N> padding, direction<N> dir){


    dimension<N> h_dims{};

    for (size_t i = 0; i < N; ++i) {
        if (dir[i] != 0) {
            h_dims[i] = padding[i];
        } 
        else {
            Utils::runtime_assert(dims[i] >= 2*padding[i], "Negative halo dimension");
            h_dims[i] = dims[i] - 2*padding[i];
        }
    }
    return h_dims;

}

template<size_t N>
static constexpr position<N> interior_begin(dimension<N> dims, dimension<N> padding, direction<N> dir) {

    position<N> begin{};
    for (size_t i = 0; i < N; ++i){
        if (dir[i] < 0) {
            begin[i] = 0;
        }

        else if (dir[i] == 0) {
            begin[i] = idx_t(padding[i]);
        }

        else {
            begin[i] = idx_t(dims[i] - padding[i]);
        }
    }
    return begin;

}


template<size_t N>
static constexpr position<N> interior_end(dimension<N> dims, dimension<N> padding, direction<N> dir) {

    return interior_begin(dims, padding, dir) + position<N>(halo_dims(dims, padding, dir));
}







}