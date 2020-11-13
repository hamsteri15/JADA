#pragma once
#include "loops/unflatten_index.hpp"
#include <array>
#include <vector>

enum class ConnectivityType { Star, Box };

namespace JADA {

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

template <size_t N, ConnectivityType CT>
std::vector<std::array<int, N>> block_neighbours() {

    if constexpr (CT == ConnectivityType::Star) {

        std::array<int, N> a_positive{};
        std::array<int, N> a_negative{};
        a_positive[0]       = 1;
        a_negative[0]       = -1;
        auto p_permutations = all_permutations_off(a_positive);
        auto n_permutations = all_permutations_off(a_negative);

        p_permutations.insert(
            p_permutations.end(), n_permutations.begin(), n_permutations.end());
        return p_permutations;
    }

    size_t n_combinations = 1;
    for (size_t i = 0; i < N; ++i) { n_combinations *= 3; }

    std::vector<std::array<int, N>>
        combinations; 
        
    // neglect the no-opt {0, 0, 0...}
    combinations.reserve(n_combinations - 1);

    std::array<int, N> combination;
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

        if (std::any_of(std::begin(combination),
                        std::end(combination),
                        [](int c) { return c != 0; })) {
            combinations.emplace_back(combination);
        }
    }

    return combinations;
}

} // namespace JADA