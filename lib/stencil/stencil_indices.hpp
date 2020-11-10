#pragma once

#include "loops/index_type.hpp"
#include "utils/constexpr_functions.hpp"
#include "utils/runtime_assert.hpp"
#include <algorithm>
#include <array>
#include <utility>

namespace JADA {

template <size_t N> using StencilIndices = std::array<int, N>;

///
///@brief Take the absolute value of the indices
///
///@tparam N size of indices
///@param idx indices to take the absolute value from
///@return constexpr StencilIndices<N> absolute value of indices
///
template <size_t N>
static inline constexpr StencilIndices<N> abs(const StencilIndices<N>& idx) {

    auto ret = idx;
    for (auto& r : ret) { r = Utils::constexpr_abs(r); }
    return ret;
}

///
///@brief Find the maximum of the indices
///
///@tparam N size of indices
///@param idx indices to find the maximum from
///@return constexpr int maximum index
///
template <size_t N>
static inline constexpr int max(const StencilIndices<N>& idx) {

    return *std::max_element(idx.begin(), idx.end());
}

///
///@brief Find the minimum of the indices
///
///@tparam N size of indices
///@param idx indices to find the maximum from
///@return constexpr int maximum index
///
template <size_t N>
static inline constexpr int min(const StencilIndices<N>& idx) {

    return *std::min_element(idx.begin(), idx.end());
}

///
///@brief Reverses the indices such that {-1,0,1} becomes {1, 0, -1}
///
///@tparam N size of the indices
///@param idx indices to reverse
///@return constexpr StencilIndices<N> reversed indices
///
template <size_t N>
static inline constexpr StencilIndices<N>
reverse(const StencilIndices<N>& idx) {

    auto ret = idx;
    std::reverse(ret.begin(), ret.end());
    return ret;
}

///
///@brief Mirrors the front of the indices leaving the indices >= pivot
///unchanged.
///       Indices {1,2,3,4,5,6} with pivot 2 become {5,4,3,4,5,6}
///
///@tparam N size of indices
///@param idx the indices to mirror
///@param pivot point to with respect the mirror operation is peformed starting
///from the _beginning_ of indices
///@return constexpr StencilIndices<N> mirrored indices
///
template <size_t N>
static inline constexpr StencilIndices<N>
mirror_front(const StencilIndices<N>& idx, idx_t pivot) {

    Utils::runtime_assert(pivot <= (idx.size() / 2), "Pivot out of bounds");
    auto ret = idx;

    std::reverse_copy(std::begin(idx) + int(pivot + 1),
                      std::begin(idx) + int(2 * pivot + 1),
                      std::begin(ret));
    return ret;
}

///
///@brief Mirrors the back of the indices leaving the indices >= pivot
///unchanged.
///       Indices {1,2,3,4,5,6} with pivot 2 become {1,2,3,4,3,2}
///
///@tparam N size of indices
///@param idx the indices to mirror
///@param pivot point to with respect the mirror operation is peformed starting
///from the _end_ of indices
///@return constexpr StencilIndices<N> mirrored indices
///
template <size_t N>
static inline constexpr StencilIndices<N>
mirror_back(const StencilIndices<N>& idx, idx_t pivot) {

    Utils::runtime_assert(pivot <= (idx.size() / 2), "Pivot out of bounds");
    auto ret = idx;

    std::reverse_copy(std::rbegin(idx) + int(pivot + 1),
                      std::rbegin(idx) + int(2 * pivot + 1),
                      std::rbegin(ret));
    return ret;
}

} // namespace JADA