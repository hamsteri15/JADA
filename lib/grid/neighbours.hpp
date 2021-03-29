#pragma once
#include "loops/position.hpp"
#include "loops/unflatten_index.hpp"
#include <array>
#include <vector>

namespace JADA {

enum class ConnectivityType { Star, Box };

namespace detail {

template <size_t N, ConnectivityType CT>
static constexpr size_t neighbour_count() {

    if constexpr (CT == ConnectivityType::Star) { return 2 * N; }

    size_t n = 1;
    for (size_t i = 0; i < N; ++i) { n *= 3; }
    return n - 1; // neglect all zeroes
}

} // namespace detail

///
///@brief Given N dimensions holds the possible neighbour directions that a
/// block may have based on the
///       connectivity type. For N=2 the neighbours for connectivity type Star
///       are: [1, 0], [-1, 0], [0,1], [0,-1]. For N = 2 and Box connectivity,
///       also the combinations are returned i.e. [1,-1] [-1, 1]...
///
///@tparam N number of spatial dimensions
///@tparam CT connectivity type Box/Star
///
template <size_t N, ConnectivityType CT> struct Neighbours {

    static constexpr auto create() {
        if constexpr (CT == ConnectivityType::Star) {
            return star_neighbours();
        } else {
            return box_neighbours();
        }
    }



    static constexpr size_t m_count = detail::neighbour_count<N, CT>();

    static constexpr std::array<std::array<idx_t, N>, m_count> m_neighbours = create();

public:



    ///
    ///@brief Get the array of neighbours
    ///
    ///@return constexpr auto array of neighbours
    ///
    static constexpr auto get() { return m_neighbours; }

    ///
    ///@brief Get the total neighbour count
    ///
    ///@return constexpr size_t 
    ///
    static constexpr size_t count() { return m_neighbours.size(); }

    ///
    ///@brief Converts a neighbour direction to an idx in the neighbours array
    ///
    ///@param neighbour direction of a neighbour
    ///@return constexpr size_t the index of the neihbour if such neigbhour is found. -1 otherwise.
    ///
    static constexpr idx_t idx(position<N> neighbour) {

        for (size_t i = 0; i < count(); ++i) {
            if (position<N>(m_neighbours[i]) == neighbour) { return idx_t(i); }
        }
        return -1;
        //throw std::logic_error("Invalid neighbour");
    }

private:
    ///
    ///@brief For a given array returns a vector of permutations of the elements
    /// of the array
    ///
    ///@tparam N Dimensions of the array
    ///@tparam ET Array element type
    ///@param arr the input array
    ///@return constexpr std::vector<std::array<ET, N>> vector of permutations
    ///
    static constexpr auto all_permutations_of(std::array<idx_t, N> arr) {

        std::array<std::array<idx_t, N>, N> permutations;

        // std::vector<std::array<idx_t, N>> permutations(count);
        auto temp = arr;
        std::sort(std::begin(temp), std::end(temp));

        size_t i = 0;
        do {
            permutations[i] = temp;
            ++i;
            // permutations.push_back(temp);
        } while (std::next_permutation(temp.begin(), temp.end()));

        return permutations;
    }

    ///
    ///@brief Computes the star neighbours
    ///
    ///@return constexpr auto std::array<std::array<idx_t, N> 2*N> neighbours
    ///
    static constexpr auto star_neighbours() {

        constexpr size_t n_count = detail::neighbour_count<N, CT>();

        std::array<idx_t, N> a_positive{};
        std::array<idx_t, N> a_negative{};
        a_positive[0]       = 1;
        a_negative[0]       = -1;
        auto p_permutations = all_permutations_of(a_positive);
        auto n_permutations = all_permutations_of(a_negative);

        std::array<std::array<idx_t, N>, n_count> all;

        for (size_t i = 0; i < p_permutations.size(); ++i) {
            all[i] = p_permutations[i];
        }

        for (size_t i = 0; i < n_permutations.size(); ++i) {
            all[i + p_permutations.size()] = n_permutations[i];
        }

        return all;
    }

    ///
    ///@brief Computes the box neighbour directions
    ///
    ///@return constexpr auto std::array<std::array<idx_t, N>, 3^N>
    ///
    static constexpr auto box_neighbours() {

        constexpr auto n_combinations = detail::neighbour_count<N, CT>();

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

            // Neglect the no-op [0,0,0...]
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