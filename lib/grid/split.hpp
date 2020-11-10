#pragma once

#include <array>
#include <vector>
#include "grid/splitting_policy_min_diff.hpp"
#include "grid/local_global_mapping.hpp"
#include "loops/index_type.hpp"
#include "loops/md_index_loops.hpp"
#include "grid/block.hpp"


namespace JADA {

///
///@brief Splits a N-dimensional box in to smaller boxes based on the policy. Throws if the splitting fails.
///
///
///@param dims dimensions of the big box
///@param n number of smaller boxes
///@return std::array<size_t, N> number of smaller boxes in each direction N
///
template <size_t N, class Policy = SplittingPolicyMinDiff<N>>
static inline std::array<idx_t, N> split(
                                         const std::array<idx_t, N>& dims,
                                         size_t n) {

    auto candidates = Policy::get_candidates(n, dims);
    if (candidates.size() == 0) {
        throw std::runtime_error("Splitting fails.");
    }

    auto optimal = Policy::find_optimal(candidates, dims);
    return optimal;
}








} // namespace JADA