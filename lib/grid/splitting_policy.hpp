#pragma once

#include "loops/index_type.hpp"

namespace JADA {

template <idx_t N> struct SplittingPolicy {

    static_assert(N <= 3, "Only up to 3 dimensions supported.");

protected:
    // all possible splitting candidates
    using candidate_array = std::vector<std::array<size_t, N>>;

public:
    ///
    ///@brief Get all possible splitting candidates
    ///
    ///@param n total number of splits
    ///@param dims global dimensions
    ///@return candidate_array<N> all possible splitting candidates
    ///
    static candidate_array get_candidates(size_t                       n,
                                          const std::array<size_t, N>& dims) {

        // TODO get rid of the if statement, should be possible

        // get all factors of n
        std::vector<size_t> factors = factor(n);

        candidate_array candidates;

        if constexpr (N == 1) {

            std::array<size_t, N> test{n};

            if (SplittingPolicy<N>::is_valid(test, dims, n)) {
                candidates.emplace_back(test);
            }
        }

        else if (N == 2) {

            for (const auto& j_factor : factors) {
                for (const auto& i_factor : factors) {

                    std::array<size_t, N> test;
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

                        std::array<size_t, N> test;

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

protected:

    ///
    ///@brief Returns all factors of n as a vector
    ///
    ///@param n the number to factor
    ///@return std::vector<idx_t> all factors
    ///
    static inline std::vector<idx_t> factor(idx_t n) {

        std::vector<idx_t> ret;

        for (idx_t i = 1; i <= n; ++i) {
            if ((n % i) == 0) { ret.push_back(i); }
        }

        return ret;
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
    static inline bool is_valid(const std::array<size_t, N>& decomposition,
                                const std::array<size_t, N>& dims,
                                size_t                       n) {

        // ensure that domaincount equal to the number of processes
        size_t total = 1;
        for (size_t i = 0; i < N; ++i) { total *= decomposition[i]; }
        if (total != n) { return false; }

        // ensure that none of the split directions exceed the global node count
        // in the corresponding direction
        for (size_t i = 0; i < N; ++i) {
            if (decomposition[i] > dims[i]) { return false; }
        }
        return true;
    }
};

} // namespace JADA