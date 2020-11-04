#pragma once

#include "loops/index_type.hpp"
#include "loops/position.hpp"
#include "loops/storage_order.hpp"
#include "utils/can_throw.hpp"
#include "utils/runtime_assert.hpp"
#include <array>
#include <numeric> //std::accumulate
#include <tuple>

namespace JADA {

template <size_t N, idx_t I, StorageOrder storage>
constexpr idx_t get_shift(std::array<idx_t, N> dimension) {

    static_assert(I < N, "Shift index out of bounds");

    if constexpr (storage == StorageOrder::RowMajor) {

        return std::accumulate(std::begin(dimension) + int(I) + 1,
                               std::end(dimension),
                               idx_t(1),
                               std::multiplies<idx_t>{});
    }

    return std::accumulate(std::begin(dimension),
                           std::begin(dimension) + int(I),
                           idx_t(1),
                           std::multiplies<idx_t>{});
}

template <size_t N, StorageOrder storage>
constexpr std::array<idx_t, N> get_multipliers(std::array<idx_t, N> arr) {
    return [&]<auto... Is>(std::index_sequence<Is...>) {
        return std::array<idx_t, N>{get_shift<N, Is, storage>(arr)...};
    }
    (std::make_integer_sequence<idx_t, N>{});
}

template <size_t N, StorageOrder storage>
constexpr idx_t flatten(std::array<idx_t, N> dimension,
                        position<N>          idx) noexcept(Utils::can_throw) {

    Utils::runtime_assert(std::equal(std::begin(idx),
                                     std::end(idx),
                                     std::begin(dimension),
                                     std::end(dimension),
                                     std::less{}),
                          "Index out of bounds");

    const auto mult = get_multipliers<N, storage>(dimension);

    idx_t index{0};
    for (idx_t i = 0; i < N; ++i) { index += idx[i] * mult[i]; }
    return index;
}

template <StorageOrder storage, class... Is>
constexpr idx_t flatten(std::array<idx_t, sizeof...(Is)> dimension,
                        Is... indices) noexcept(Utils::can_throw) {
    return flatten<sizeof...(Is), storage>(
        dimension, std::to_array<idx_t>({idx_t(indices)...}));
}

} // namespace JADA
