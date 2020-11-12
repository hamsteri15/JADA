#pragma once
#include <array>
#include <vector>

#include "utils/permutations.hpp"

enum class ConnectivityType{
    Star, Box
};

namespace JADA{

template <class T, size_t N, size_t R>
void getPermutations(const std::array<T, N>&        arr,
                     std::array<T, R>&              permutation,
                     size_t                            curIndex,
                     std::vector<std::array<T, R>>& permutations) {
    // stop recursion condition
    if (curIndex == R) {
        permutations.emplace_back(permutation);

    } else {
        for (size_t i = 0; i < N; i++) {
            permutation[curIndex] = arr[i];
            getPermutations(arr, permutation, curIndex + 1, permutations);
        }
    }
}



// N = in array length
// R = permutation array length
template <class T, size_t N, size_t R> auto all_possible_permutations(const std::array<T, N>& arr) {

    std::array<T, R>              permutation;
    std::vector<std::array<T, R>> permutations;

    size_t curIndex = 0;
    getPermutations(arr, permutation, curIndex, permutations);

    return permutations;
}


template<size_t N, ConnectivityType CT>
std::vector<std::array<int, N>> block_neighbours() {



    if constexpr (CT == ConnectivityType::Star){



        std::array<int, N> a_positive{};
        std::array<int, N> a_negative{};
        a_positive[0] = 1;
        a_negative[0] = -1;
        auto p_permutations = Utils::all_permutations_off(a_positive);
        auto n_permutations = Utils::all_permutations_off(a_negative);

        p_permutations.insert(p_permutations.end(), n_permutations.begin(), n_permutations.end());
        return p_permutations;

    }

    

    
    auto permutations = all_possible_permutations<int, 3, N>(std::array<int, 3>{1, 0, -1});
    std::vector<std::array<int, N>> good_permutations;
    
    for (auto p : permutations){
        
        if (
            std::any_of(p.begin(), p.end(), [](int i) {return i != 0;})
        )
        {
            good_permutations.push_back(p);
        }

    }   

    return good_permutations;    


//    return permutations;
    /*std::array<int, 3> possibilities = {1, 0, -1};

    return all_possible_permutations<int, 3, N>(possibilities);
    */
}







}