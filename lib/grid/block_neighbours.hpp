#pragma once
#include "loops/unflatten_index.hpp"
#include "loops/position.hpp"
#include <array>
#include <vector>


namespace JADA {

enum class ConnectivityType { Star, Box };

///
///@brief Given N dimensions holds the possible neighbour directions that a block may have based on the
///       connectivity type. For N=2 the neighbours for connectivity type Star are: 
///       [1, 0], [-1, 0], [0,1], [0,-1]. 
///       For N = 2 and Box connectivity, also the combinations are returned i.e. [1,-1] [-1, 1]...
///
///@tparam N number of spatial dimensions
///@tparam CT connectivity type Box/Star
///
template<size_t N, ConnectivityType CT>
struct BlockNeighbours{


    constexpr BlockNeighbours() {
        if constexpr (CT == ConnectivityType::Star) {
            m_neighbours = star_neighbours();
        }
        else {
            m_neighbours = box_neighbours();
        }
    }

    constexpr auto get() const {
        return m_neighbours;
    }


    constexpr size_t count() const {
        return m_count;
    }


    constexpr size_t idx(position<N> neighbour) const {

        for (size_t i = 0; i < count(); ++i){
            if (position<N>(m_neighbours[i]) == neighbour) {return i;}
        }
        throw std::logic_error("Invalid neighbour");

    }

    static constexpr size_t neighbour_count() {

    if constexpr (CT == ConnectivityType::Star) {
        return 2*N;        
    }

    size_t n = 1;
    for (size_t i = 0; i < N; ++i) {n *= 3;}
    return n - 1; //neglect all zeroes

    }

private:

    static constexpr size_t m_count = neighbour_count();
    std::array<std::array<idx_t, N>, m_count> m_neighbours;




///
///@brief For a given array returns a vector of permutations of the elements of the array
///
///@tparam N Dimensions of the array
///@tparam ET Array element type
///@param arr the input array
///@return constexpr std::vector<std::array<ET, N>> vector of permutations
///
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






static constexpr auto star_neighbours() {

    constexpr size_t n_count = neighbour_count();

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

static constexpr auto box_neighbours() {

    
    constexpr auto n_combinations = neighbour_count();

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








};











} // namespace JADA