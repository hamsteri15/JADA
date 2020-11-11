#pragma once
#include <array>
#include <vector>

#include "utils/permutations.hpp"

enum class ConnectivityType{
    Star, Box
};

namespace JADA{

template<size_t N, ConnectivityType CT>
constexpr std::vector<std::array<int, N>> block_neighbours() {

    if constexpr (CT == ConnectivityType::Star){
        std::array<int, N> arr{};
        arr[0] = 1;
        return Utils::all_permutations_off(arr);
    }

    //Box-type connectivity

    std::vector<std::array<int, N>> permutations;
    std::array<int, N> arr{};
    for (auto& a : arr){
        a = 1;
        auto local_p = Utils::all_permutations_off(arr);
        permutations.insert(permutations.begin(), local_p.begin(), local_p.end());
    }

    return permutations;
}




}