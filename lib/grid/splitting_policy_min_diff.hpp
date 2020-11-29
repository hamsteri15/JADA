#pragma once
#include <algorithm>
#include "loops/dimension.hpp"
#include "grid/splitting_policy.hpp"



namespace JADA {

// This policy minimizes the global difference of the dimensions of the small boxes.

template <size_t N> class SplittingPolicyMinDiff : public SplittingPolicy<N> {


    using typename SplittingPolicy<N>::candidate_array;

public:
    
    ///
    ///@brief Given an array of candidate splittings, finds the optimal one
    ///
    ///@param candidates vector of candidates to optimize, assumes all are valid
    ///@param dims global dimensions
    ///@return optimal dimensions
    ///
    static auto
    find_optimal(const candidate_array&       candidates,
                 dimension<N> dims) {

        auto is_more_square = [&](const dimension<N>& lhs,
                                  const dimension<N>& rhs) {

            auto lhs_dims = dims / lhs;
            auto rhs_dims = dims / rhs;


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
    static size_t sum_abs_diff(const dimension<N>& arr) {

        size_t diff = 0;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                diff += size_t (std::abs(long(arr[i]) - long(arr[j]))); 
            }
        }

        return diff;
    }

};

} // namespace JADA
