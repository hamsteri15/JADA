#pragma once

#include "loops/index_type.hpp"
#include "loops/flatten_index.hpp"
#include "loops/storage_order.hpp"
#include "utils/can_throw.hpp"
#include "utils/runtime_assert.hpp"
#include <array>
#include <numeric> //std::accumulate

namespace JADA {

///
///@brief Given dimensions and an index returns the multidimensional index based on the storage order
///
///@tparam N number of dimensions
///@tparam storage storage order
///@param dims maximum extent of the multidimensional indices
///@param idx index to unflatten
///@return constexpr std::array<idx_t, N> array of multidimensional indices
///
template <size_t N, StorageOrder storage>
constexpr std::array<idx_t, N> unflatten(std::array<idx_t, N> dims,
                                         idx_t idx) noexcept(Utils::can_throw) {

    Utils::runtime_assert(std::accumulate(std::begin(dims),
                                          std::end(dims),
                                          idx_t(1),
                                          std::multiplies<idx_t>{}) > idx,
                          "Index out of bounds"

    );

    std::array<idx_t, N> md_idx;
    auto                 mult = get_shifts<N, storage>(dims);

    if constexpr (storage == StorageOrder::RowMajor) {

        for (idx_t i = 0; i < N; ++i) {
            md_idx[i] = idx / mult[i];
            idx -= md_idx[i] * mult[i];
        }
    }

    else {

        for (idx_t i = N - 1; int(i) >= 0; --i) {
            md_idx[i] = idx / mult[i];
            idx -= md_idx[i] * mult[i];
        }
    }

    return md_idx;
}

} // namespace JADA