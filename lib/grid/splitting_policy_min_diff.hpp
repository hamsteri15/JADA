#pragma once

#include "grid/splitting_policy.hpp"



namespace JADA {

// This policy minimizes the global difference of the dimensions of the small boxes.

template <idx_t N> class SplittingPolicyMinDiff : public SplittingPolicy<N> {


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

        /*
        std::vector<idx_t> absdiffs(candidates.size());

        idx_t i = 0;
        for (auto& candidate : candidates) {

            // approximation for the local nodecount, truncation here is ok!
            std::array<idx_t, N> local_nc{};
            for (idx_t ii = 0; ii < N; ++ii){
                local_nc[ii] = dims[ii] / candidate[ii];
            }
            absdiffs[i]   = sum_abs_diff(local_nc);
            i++;
        }
        */
        auto square = [&](const std::array<idx_t, N>& lhs, const std::array<idx_t, N> rhs){

            std::array<idx_t, N> lhs_dims{};
            std::array<idx_t, N> rhs_dims{};
            for (idx_t i = 0; i < N; ++i){
                lhs_dims[i] = dims[i] / lhs[i];
                rhs_dims[i] = dims[i] / rhs[i];
            }

            return sum_abs_diff(lhs_dims) < sum_abs_diff(rhs_dims);
            //return sum_abs_diff(local_dims);

        };


        return *std::min_element(candidates.begin(), candidates.end(), square);
        /*
        idx_t idx = std::min_element(absdiffs.begin(), absdiffs.end()) -
                     absdiffs.begin();

        return candidates[idx];
        */
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
