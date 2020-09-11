#pragma once

#include <array>
#include "grid/splitting_policy_min_diff.hpp"
#include "loops/index_type.hpp"

namespace JADA {

///
///@brief Splits a N-dimensional box in to smaller boxes based on the policy. Throws if the splitting fails.
///
///
///@param n number of smaller boxes
///@param dims dimensions of the big box
///@return std::array<size_t, N> number of smaller boxes in each direction N
///
template <idx_t N, class Policy = SplittingPolicyMinDiff<N>>
static inline std::array<idx_t, N> split(size_t                      n,
                                         const std::array<idx_t, N>& dims) {

    auto candidates = Policy::get_candidates(n, dims);
    if (candidates.size() == 0) {
        throw std::runtime_error("Splitting fails.");
    }

    auto optimal = Policy::find_optimal(candidates, dims);
    return optimal;
}



} // namespace JADA