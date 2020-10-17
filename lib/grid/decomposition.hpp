#pragma once

#include <array>
#include <vector>

#include "grid/split.hpp"
#include "grid/splitting_policy_min_diff.hpp"
#include "grid/local_global_mapping.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <size_t N> class Decomposition : private LocalGlobalMapping<N> {

    static_assert(N <= 3, "Only up to 3 dimensions supported.");

    using splitting_policy_t = SplittingPolicyMinDiff<N>;

    size_t                m_subdomain_count;     // total number of subdomains
    std::array<size_t, N> m_global_dims;         // global domain dimensions
    std::array<size_t, N> m_periodic_directions; // periodic (1) or not in each direction (0)
    std::array<size_t, N> m_barriers;            // direction specific number of barriers
    std::array<size_t, N> m_subdomain_counts;    // direction specific total number of subdomains

public:
    Decomposition() = default;


    ///
    ///@brief Construct from specified number of splitst
    ///
    ///@param subdomain_count total number of subdomains
    ///@param global_dims total number of subdomains 
    ///@param periodic_directions periodic (1) or not (0) in each direction 
    ///@param barriers number of barrier nodes in each direction
    ///@param subdomain_counts speciefied splitting in each direction
    ///
    Decomposition(size_t                       subdomain_count,
                  const std::array<size_t, N>& global_dims,
                  const std::array<size_t, N>& periodic_directions,
                  const std::array<size_t, N>& barriers,
                  const std::array<size_t, N>& subdomain_counts)
        : m_subdomain_count(subdomain_count)
        , m_global_dims(global_dims)
        , m_periodic_directions(periodic_directions)
        , m_barriers(barriers)
        , m_subdomain_counts(subdomain_counts) {
        Utils::runtime_assert(
            splitting_policy_t::is_valid(subdomain_counts, global_dims, subdomain_count),
            "Invalid domain decomposition.");
    }

    

    ///
    ///@brief Construct from subdomain count and global dimensions. The splitting policy finds an
    /// optimal splitting.
    ///
    ///@param subdomain_count total number of subdomains
    ///@param global_dims total number of subdomains 
    ///@param periodic_directions periodicity periodic (1) or not (0) in each direction 
    ///@param barriers number of barrier nodes in each direction
    ///
    Decomposition(size_t                       subdomain_count,
                  const std::array<size_t, N>& global_dims,
                  const std::array<size_t, N>& periodic_directions,
                  const std::array<size_t, N>& barriers)
        : Decomposition(subdomain_count,
                        global_dims,
                        periodic_directions,
                        barriers,
                        split(global_dims, subdomain_count)) {}


    ///
    ///@brief Get the total number of subdomains
    ///
    ///@return size_t total number of subdomains
    ///
    size_t get_subdomain_count() const { return m_subdomain_count; }

    ///
    ///@brief Get the number of subdomains in each direction
    ///
    ///@return const std::array<size_t, N>& number of subdomains in each direction
    ///
    const std::array<size_t, N>& get_subdomain_counts() const { return m_subdomain_counts; }

    ///
    ///@brief Get directionwise peridicity information
    ///
    ///@return const std::array<size_t, N>& periodic (1) or not (0) in each direction
    ///
    const std::array<size_t, N>& get_periodic_directions() const { return m_periodic_directions; }

    ///
    ///@brief Get the directionwise global dimensions
    ///
    ///@return const std::array<size_t, N>& direction wise global (array) dimensions
    ///
    const std::array<size_t, N>& get_global_dimensions() const { return m_global_dims; }

    ///
    ///@brief Get the directionwise barriers count information
    ///
    ///@return const std::array<size_t, N>& direction wise barrier dimensions
    ///
    const std::array<size_t, N>& get_barriers() const { return m_barriers; }

    ///
    ///@brief Get the directionwise local dimensions of the subdomain at coords
    ///
    ///@param coords Location of the subdomain w.r.t the decomposition topology
    ///@return std::array<size_t, N> direction wise local (array) dimensions
    ///
    std::array<size_t, N> get_local_dimensions(const std::array<size_t, N>& coords) const {
        return this->local_extent(m_global_dims, m_subdomain_counts, coords);
    }

    ///
    ///@brief Get the starting index of this process w.r.t to global dimensions
    ///
    ///@param coords Location of the subdomain w.r.t the decomposition topology
    ///@return std::array<size_t, N> the start index
    ///
    std::array<size_t, N> get_start(const std::array<size_t, N>& coords) const {

        return this->start(m_global_dims, m_subdomain_counts, coords);
    }
};

} // namespace JADA