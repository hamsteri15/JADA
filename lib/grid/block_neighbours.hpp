#pragma once
#include "loops/unflatten_index.hpp"
#include <array>
#include <vector>


namespace JADA {

enum class ConnectivityType { Star, Box };

/*
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
*/

///
///@brief For a given array returns a vector of permutations of the elements of the array
///
///@tparam N Dimensions of the array
///@tparam ET Array element type
///@param arr the input array
///@return constexpr std::vector<std::array<ET, N>> vector of permutations
///
template<size_t N>
static constexpr auto all_permutations_of(std::array<idx_t, N> arr) {

    constexpr size_t count = N;

    std::array<std::array<idx_t, N>, count> permutations;

    //std::vector<std::array<idx_t, N>> permutations(count);
    auto temp = arr;
    std::sort(std::begin(temp), std::end(temp));

    size_t i = 0;
    do {
        permutations[i] = temp;
        ++i;
        //permutations.push_back(temp);
    } while (std::next_permutation(temp.begin(), temp.end()));

    return permutations;

}



template<size_t N, ConnectivityType CT>
static constexpr size_t neighbour_count() {

    if constexpr (CT == ConnectivityType::Star) {
        return 2*N;        
    }

    size_t n = 1;
    for (size_t i = 0; i < N; ++i) {n *= 3;}
    return n - 1; //neglect all zeroes

}


template<size_t N>
static constexpr auto star_neighbours() {

    constexpr size_t n_count = neighbour_count<N, ConnectivityType::Star>();

    std::array<idx_t, N> a_positive{};
    std::array<idx_t, N> a_negative{};
    a_positive[0]       = 1;
    a_negative[0]       = -1;
    auto p_permutations = all_permutations_of(a_positive);
    auto n_permutations = all_permutations_of(a_negative);

    std::array<std::array<idx_t, N>, n_count> all;

    for (size_t i = 0; i < p_permutations.size(); ++i){
        all[i] = p_permutations[i];
    }

    for (size_t i = 0; i < n_permutations.size(); ++i){
        all[i+p_permutations.size()] = n_permutations[i];
    }


    return all;


}

template<size_t N>
static constexpr auto box_neighbours() {

    
    constexpr auto n_combinations = neighbour_count<N, ConnectivityType::Box>();

    std::array<std::array<idx_t, N>, n_combinations> combinations;

    std::array<idx_t, N> combination;
    for (auto& c : combination) c = 1;


    size_t j = 0;
    for (size_t i = 0; i < n_combinations + 1; ++i) {

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
            combinations[j] = combination;
            ++j;
        }
    }

    return combinations;
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
static constexpr auto block_neighbours() {

    if constexpr (CT == ConnectivityType::Star) {
        return star_neighbours<N>();
    }

    throw std::logic_error("For now");
    //return box_neighbours<N>();

    //Box type connectivity


}










} // namespace JADA