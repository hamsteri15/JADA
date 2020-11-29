#pragma once

#include <algorithm>
#include <numeric>
#include "loops/index_type.hpp"
#include "loops/dimension.hpp"
#include "loops/md_index_loops.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <size_t N> struct SplittingPolicy {

    static_assert(N <= 3, "Only up to 3 dimensions supported.");

protected:
    // all possible splitting candidates
    using candidate_array = std::vector<dimension<N>>;

public:
    ///
    ///@brief Get all possible splitting candidates
    ///
    ///@param n total number of splits
    ///@param dims global dimensions
    ///@return candidate_array<N> all possible splitting candidates
    ///
    static candidate_array get_candidates(size_t                       n,
                                          dimension<N> dims) {

        // TODO get rid of the if statement, should be possible

        
        candidate_array candidates;

        /*
        //This works but is very slow

        candidates.reserve(n*n*n);

        std::array<idx_t, N> min;
        std::array<idx_t, N> max;
        for (auto& m : min) {m = 1;}
        for (auto& m : max) {m = n + 1;}



        for (auto test : md_indices(min, max)){
            if (SplittingPolicy<N>::is_valid(test, dims n)) {
                candidates.emplace_back(test);
            }
        }
        */
       
        
        // get all factors of n
        std::vector<size_t> factors = factor(n);

        if constexpr (N == 1) {

            dimension<N> test{n};

            if (SplittingPolicy<N>::is_valid(test, dims, n)) {
                candidates.emplace_back(test);
            }
        }

        else if (N == 2) {

            for (const auto& j_factor : factors) {
                for (const auto& i_factor : factors) {

                    dimension<N> test;
                    test[0] = j_factor;
                    test[1] = i_factor;
                    if (SplittingPolicy<N>::is_valid(test, dims, n)) {
                        candidates.emplace_back(test);
                    }
                }
            }
        }

        // N == 3
        else {

            for (const auto& k_factor : factors) {
                for (const auto& j_factor : factors) {
                    for (const auto& i_factor : factors) {

                        dimension<N> test;

                        test[0] = k_factor;
                        test[1] = j_factor;
                        test[2] = i_factor;
                        if (SplittingPolicy<N>::is_valid(test, dims, n)) {
                            candidates.emplace_back(test);
                        }
                    }
                }
            }
        }
        

        return candidates;
    }

    
    ///
    ///@brief Checks is a given decomposition is valid
    ///
    ///@param decomposition number of splits in each direction
    ///@param dims global dimensions
    ///@param n total number of splits
    ///@return true if splitting is valid
    ///@return false if splitting is invalid
    ///
    static inline bool is_valid(dimension<N> decomposition,
                                size_t                       n) {

        return decomposition.elementwise_product() == n;
    }
    
    ///
    ///@brief Checks is a given decomposition is valid
    ///
    ///@param decomposition number of splits in each direction
    ///@param dims global dimensions
    ///@param n total number of splits
    ///@return true if splitting is valid
    ///@return false if splitting is invalid
    ///
    static inline bool is_valid(dimension<N> decomposition,
                                dimension<N> dims,
                                size_t                       n) {

        return (decomposition.elementwise_product() == n &&
                std::ranges::equal(decomposition, dims, std::less_equal{}));
    }

protected:

    
    ///
    ///@brief Returns all factors of an integer n
    ///
    ///@tparam INT an integer type
    ///@param n the number to factor
    ///@return std::vector<INT> vector of factors
    ///
    template<class INT>
    static inline std::vector<INT> factor(INT n) {

        std::vector<INT> ret;

        for (INT i = 1; i <= n; ++i) {
            if ((n % i) == 0) { ret.push_back(i); }
        }

        return ret;
    }

};

} // namespace JADA