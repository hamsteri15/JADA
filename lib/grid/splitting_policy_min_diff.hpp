#pragma once
#include <algorithm>
#include "grid/splitting_policy.hpp"



namespace JADA {

// This policy minimizes the global difference of the dimensions of the small boxes.

template <size_t N> class SplittingPolicyMinDiff : public SplittingPolicy<N> {


    using typename SplittingPolicy<N>::candidate_array;

public:
    
    ///
    ///@brief Given an array of candidate splittings, finds the optimal one
    ///
    ///@param candidates
    ///@param dims
    ///@return std::array<idx_t, N>
    ///
    static std::array<idx_t, N>
    find_optimal(const candidate_array&       candidates,
                 const std::array<idx_t, N>& dims) {

        auto is_more_square = [&](const std::array<idx_t, N>& lhs,
                                  const std::array<idx_t, N>& rhs) {
            std::array<idx_t, N> lhs_dims{};
            std::array<idx_t, N> rhs_dims{};

            std::transform(std::cbegin(dims),
                           std::cend(dims),
                           std::cbegin(lhs),
                           std::begin(lhs_dims),
                           std::divides<>{});
            std::transform(std::cbegin(dims),
                           std::cend(dims),
                           std::cbegin(rhs),
                           std::begin(lhs_dims),
                           std::divides<>{});


            return sum_abs_diff(lhs_dims) < sum_abs_diff(rhs_dims);
        };

        return *std::min_element(candidates.begin(), candidates.end(), is_more_square);
    }



private:

    ///
    ///@brief For a given array, sums up the difference between each element i
    /// with neighbours j
    ///
    ///@param arr the array to process
    ///@return idx_t absolute difference between each element and its
    /// neighbours
    ///
    static idx_t sum_abs_diff(const std::array<idx_t, N>& arr) {

        idx_t diff = 0;
        for (idx_t i = 0; i < N; ++i) {
            for (idx_t j = 0; j < N; ++j) {
                diff += idx_t (std::abs(long(arr[i]) - long(arr[j]))); 
            }
        }

        return diff;
    }

};

} // namespace JADA
