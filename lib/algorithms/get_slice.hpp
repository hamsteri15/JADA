#pragma once

#include <vector>

#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/flatten_index.hpp"
#include "loops/md_index_loops.hpp"
#include "utils/runtime_assert.hpp"




namespace JADA{


///
///@brief Gets a slice of size end - begin from input parameter 'in'
///
///@tparam N number of spatial dimensions
///@tparam T element type of input / output data
///@param in the data to get the slice from
///@param dim dimensions of the input 'in' data
///@param begin beginning of the slice
///@param end   end of the slice
///@return std::vector<T> the sliced data
///
template<size_t N, class T>
std::vector<T> get_slice(const std::vector<T>& in, dimension<N> dim, position<N> begin, position<N> end) {

    dimension<N> slice_dims{};

    for (size_t i = 0; i < N; ++i){
        Utils::runtime_assert(begin[i] <= end[i], "Invalid slice dimensions");
        slice_dims[i] = size_t(end[i] - begin[i]);
    }

    std::vector<T> slice(slice_dims.elementwise_product());

    for (auto pos : md_indices(begin, end)) {
        auto slice_pos = pos - begin;
        size_t slice_idx = size_t(flatten<N, StorageOrder::RowMajor>(slice_dims, slice_pos));
        size_t in_idx = size_t(flatten<N, StorageOrder::RowMajor>(dim, pos));

        slice[slice_idx]  = in[in_idx];

    }

    return slice;

}


}

