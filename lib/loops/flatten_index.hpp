#pragma once

#include "loops/index_type.hpp"
#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/storage_order.hpp"
#include "utils/can_throw.hpp"
#include "utils/runtime_assert.hpp"
#include <array>
#include <numeric> //std::accumulate

namespace JADA {

///
///@brief Get the shift between consequtive indices in direction I based on the
/// storage order
///
///@tparam N number of dimensions
///@tparam I the component (direction) to get the shift for
///@tparam storage the storage order
///@param dimension the maximum extent of the multidimensional array
///@return constexpr idx_t the shift in direction I
///
template <size_t N, size_t I, StorageOrder storage>
constexpr size_t get_shift(dimension<N> dim) {

    static_assert(I < N, "Shift index out of bounds");

    if constexpr (storage == StorageOrder::RowMajor) {

        return std::accumulate(std::begin(dim) + I + 1,
                               std::end(dim),
                               size_t(1),
                               std::multiplies<size_t>{});
    }

    return std::accumulate(std::begin(dim),
                           std::begin(dim) + I,
                           size_t(1),
                           std::multiplies<size_t>{});
}

///
///@brief Get the shifts between consequtive indices in all N directions of a
/// multidimensional (flattened) array
///
///@tparam N number of dimensions
///@tparam storage storage order
///@param dimension the maximum extent of the multidimensional array
///@return constexpr std::array<idx_t, N> the shifts in all N directions
///
template <size_t N, StorageOrder storage>
constexpr dimension<N> get_shifts(dimension<N> dim) {
    return [&]<auto... Is>(std::index_sequence<Is...>) {
        return dimension<N>{get_shift<N, Is, storage>(dim)...};
    }
    (std::make_index_sequence<N>{});
}

///
///@brief Given an array of multidimensional indices ([k,j,i] for example)
/// computes the flat index based on the storage order and dimensions
///
///@tparam N number of dimensions
///@tparam storage storage order
///@param dimension the maximum extent of the multidimensional array
///@param idx array of indices to flatten
///@return constexpr idx_t the flat index
///
template <size_t N, StorageOrder storage>
constexpr idx_t flatten(dimension<N> dim,
                        position<N> idx) noexcept(Utils::can_throw) {

    Utils::runtime_assert(std::equal(std::begin(idx),
                                     std::end(idx),
                                     std::begin(dim),
                                     std::end(dim),
                                     std::less{}),
                          "Index out of bounds");

    const auto mult = get_shifts<N, storage>(dim);

    return std::inner_product(
        std::begin(idx), std::end(idx), std::begin(mult), idx_t(0));
}

///
///@brief Given N indices computes the flat index based on the storage order and
/// dimensions
///
///@tparam storage storage order
///@tparam Is type of the indices
///@param dimension the maximum extent of the multidimensional array
///@param idx the indices to flatten
///@return constexpr idx_t the flat index
///
template <StorageOrder storage, class... Is>
constexpr idx_t flatten(dimension<sizeof...(Is)> dim,
                        Is... idx) noexcept(Utils::can_throw) {
    return flatten<sizeof...(Is), storage>(
        dim, position<sizeof...(Is)>{idx_t(idx)...});
        //dim, std::to_array<idx_t>({idx_t(idx)...}));
}

} // namespace JADA
