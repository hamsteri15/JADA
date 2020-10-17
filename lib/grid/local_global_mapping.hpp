#pragma once

#include <array>
#include "utils/array_op_overloads.hpp"

namespace JADA {


//TODO: rename this class 
template <size_t N> class LocalGlobalMapping {

public:
    ///
    ///@brief Gets the start and end indices of a local subdomain at subdomain_coords. Note! For
    ///       uneven splits the largest coordinate in each direction is padded with
    ///       the excess amount.
    ///
    ///@param global_dimensions global node count
    ///@param domain_splits number of domain splits in each direction
    ///@param subdomain_coords subdomain coordinates
    ///@return std::pair a pair containing the start (first) and end (second)
    ///
    static auto get_bounds(const std::array<size_t, N>& global_dimensions,
                    const std::array<size_t, N>& domain_splits,
                    const std::array<size_t, N>& subdomain_coords) {

        using namespace Utils::ArrayOpOverloads;

        return std::make_pair<std::array<size_t, N>, std::array<size_t, N>>(
            subdomain_coords * global_dimensions / domain_splits,
            (subdomain_coords + size_t(1)) * global_dimensions / domain_splits);
    }

    ///
    ///@brief Computes the local extent (nodecount) of a subdomain at subdomain_coords.
    ///
    ///@param global_dimensions global node count
    ///@param domain_splits number of domain splits in each direction
    ///@param subdomain_coords subdomain coordinates
    ///@return std::array<size_t, N> local extent in each direction
    ///
    static std::array<size_t, N> local_extent(const std::array<size_t, N>& global_dimensions,
                                       const std::array<size_t, N>& domain_splits,
                                       const std::array<size_t, N>& subdomain_coords) {
        using namespace Utils::ArrayOpOverloads;

        const auto bounds = get_bounds(global_dimensions, domain_splits, subdomain_coords);
        return bounds.second - bounds.first;
    }

    ///
    ///@brief Finds the start of a subdomain at subdomain_coords relative to the global dimensions.
    ///
    ///@param global_dimensions global node count
    ///@param domain_splits number of domain splits in each direction
    ///@param subdomain_coords subdomain coordinates
    ///@return std::array<size_t, N> first index of the subdomain w.r.t to global indices
    ///
    static std::array<size_t, N> start(const std::array<size_t, N>& global_dimensions,
                                const std::array<size_t, N>& domain_splits,
                                const std::array<size_t, N>& subdomain_coords) {

        return get_bounds(global_dimensions, domain_splits, subdomain_coords).first;
    }

    ///
    ///@brief Finds the coordinates of the subdomain containing the global index.
    ///
    ///@param global_dimensions global node count
    ///@param domain_splits number of domain splits in each direction
    ///@param global_idx the indices to determine the coordinates from
    ///@return std::array<size_t, N> coordinates of the subdomain containing global_idx
    ///
    static std::array<size_t, N> find_subdomain_coords(const std::array<size_t, N>& global_dimensions,
                                                const std::array<size_t, N>& domain_splits,
                                                const std::array<size_t, N>& global_idx) {

        using namespace Utils::ArrayOpOverloads;

        auto nc = global_dimensions / domain_splits;



        // note! for uneven splittings this will return a coordinate index one too many,
        // the special case is handled below
        auto subdomain_coords = global_idx / nc;

        for (size_t i = 0; i < N; ++i) {
            if (subdomain_coords[i] == domain_splits[i]) { subdomain_coords[i] -= size_t(1); }
        }
        return subdomain_coords;
    }

    ///
    ///@brief Maps local indices to global indices.
    ///
    ///@param global_dimensions global node count
    ///@param domain_splits number of domain splits in each direction
    ///@param subdomain_coords subdomain coordinates
    ///@param local_idx local indices to convert
    ///@return std::array<size_t, N> global indices
    ///
    static std::array<size_t, N> local_to_global(const std::array<size_t, N>& global_dimensions,
                                          const std::array<size_t, N>& domain_splits,
                                          const std::array<size_t, N>& subdomain_coords,
                                          const std::array<size_t, N>& local_idx) {

        using namespace Utils::ArrayOpOverloads;

        return start(global_dimensions, domain_splits, subdomain_coords) + local_idx;
    }

    ///
    ///@brief Maps global indices to subdomain coordinates and local indices in the subdomain.
    ///
    ///@param global_dimensions global node count
    ///@param domain_splits number of domain splits in each direction
    ///@param global_idx global indices to convert
    ///@return std::pair containing the subdomain coordinates (first) and the local indices (second)
    ///
    static auto global_to_local(const std::array<size_t, N>& global_dimensions,
                         const std::array<size_t, N>& domain_splits,
                         const std::array<size_t, N>& global_idx) {

        using namespace Utils::ArrayOpOverloads;

        auto subdomain_coords = find_subdomain_coords(global_dimensions, domain_splits, global_idx);
        auto local_idx = global_idx - start(global_dimensions, domain_splits, subdomain_coords);

        return std::make_pair(subdomain_coords, local_idx);
    }
};

} // namespace Communication