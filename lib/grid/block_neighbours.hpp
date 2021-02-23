#pragma once
#include "loops/unflatten_index.hpp"
#include <array>
#include <vector>


namespace JADA {

enum class ConnectivityType { Star, Box };

///
///@brief For a given array returns a vector of permutations of the elements of the array
///
///@tparam N Dimensions of the array
///@tparam ET Array element type
///@param arr the input array
///@return constexpr std::vector<std::array<ET, N>> vector of permutations
///
template <size_t N, class ET>
constexpr std::vector<std::array<ET, N>>
all_permutations_off(std::array<ET, N> arr) {

    // TODO: preallocate
    std::vector<std::array<ET, N>> permutations;

    auto temp = arr;
    std::sort(std::begin(temp), std::end(temp));
    do {
        permutations.push_back(temp);
    } while (std::next_permutation(temp.begin(), temp.end()));

    return permutations;
}



///
///@brief Given N dimensions returns the possible neighbour directions that a block may have based on the
///       connectivity type. For N=2 the neighbours for connectivity type Star are: 
///       [1, 0], [-1, 0], [0,1], [0,-1]. 
///       For N = 2 and Box connectivity, also the combinations are returned i.e. [1,-1] [-1, 1]...
///
///@tparam N number of dimensions
///@tparam CT Connectivity type
///@return std::vector<std::array<idx_t, N>> vector of neighbour directions
///
template <size_t N, ConnectivityType CT>
std::vector<std::array<idx_t, N>> block_neighbours() {

    if constexpr (CT == ConnectivityType::Star) {

        std::array<idx_t, N> a_positive{};
        std::array<idx_t, N> a_negative{};
        a_positive[0]       = 1;
        a_negative[0]       = -1;
        auto p_permutations = all_permutations_off(a_positive);
        auto n_permutations = all_permutations_off(a_negative);

        p_permutations.insert(
            p_permutations.end(), n_permutations.begin(), n_permutations.end());
        return p_permutations;
    }


    //Box type connectivity

    size_t n_combinations = 1;
    for (size_t i = 0; i < N; ++i) { n_combinations *= 3; }

    std::vector<std::array<idx_t, N>>
        combinations; 
        
    // neglect the no-op {0, 0, 0...}
    combinations.reserve(n_combinations - 1);

    std::array<idx_t, N> combination;
    for (auto& c : combination) c = 1;

    for (size_t i = 0; i < n_combinations; ++i) {

        for (auto& c : combination) {
            if (c != -1) {
                c -= 1;
                break;
            } else {
                c = 1;
            }
        }

        //Neglect the no-op [0,0,0...]
        if (std::any_of(std::begin(combination),
                        std::end(combination),
                        [](idx_t c) { return c != 0; })) {
            combinations.emplace_back(combination);
        }
    }

    return combinations;
}

} // namespace JADA