#pragma once

#include <array>
#include <vector>
#include <algorithm>

namespace JADA::Utils{

template<size_t N, class ET>
constexpr std::vector<std::array<ET, N>> all_permutations_off(std::array<ET, N> arr){

    //TODO: preallocate
    std::vector<std::array<ET, N>> permutations;

    auto temp = arr;
    std::sort(std::begin(temp), std::end(temp));
    do {
        permutations.push_back(temp);
    } while(std::next_permutation(temp.begin(), temp.end()));

    return permutations;

}

}