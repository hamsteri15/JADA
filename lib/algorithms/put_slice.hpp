#pragma once

#include <vector>

#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/flatten_index.hpp"
#include "loops/md_index_loops.hpp"
#include "utils/runtime_assert.hpp"




namespace JADA{


template<size_t N, class T>
void put_slice(const std::vector<T>& slice, std::vector<T>& out, dimension<N> dims, position<N> begin, position<N> end) {


    throw std::logic_error("put_slice not implemented");

    /*
    dimension<N> slice_dims{};

    for (size_t i = 0; i < N; ++i){
        Utils::runtime_assert(begin[i] <= end[i], "Invalid slice dimensions");
        slice_dims[i] = size_t(end[i] - begin[i]);
    }

    std::vector<T> slice(slice_dims.elementwise_product());

    for (auto pos : md_indices(begin, end)) {
        auto slice_pos = pos - begin;
        size_t slice_idx = size_t(flatten<N, StorageOrder::RowMajor>(slice_dims, slice_pos));
        size_t in_idx = size_t(flatten<N, StorageOrder::RowMajor>(dims, pos));

        slice[slice_idx]  = in[in_idx];

    }

    return slice;
    */
}


}