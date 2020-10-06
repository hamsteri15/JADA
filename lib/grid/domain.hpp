#pragma once

#include <utility> //std::pair

#include "grid/decomposition.hpp"
#include "grid/boundary.hpp"
#include "grid/direction.hpp"
#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <idx_t Dim> struct Domain {

    using boundary_array = std::array<std::pair<Boundary, Boundary>, Dim>;


    Domain(
           std::array<idx_t, Dim>            node_count,
           boundary_array boundary_types)
        : m_decomposition(1, node_count, periodic_directions(boundary_types), std::array<idx_t, Dim>{}) 
        , m_boundaries(boundary_types) {}


    /*

    Domain(idx_t n_domains,
           std::array<double, Dim> gphys_dims,
           std::array<idx_t, Dim> gnode_count,
           boundary_array gboundary_types)
           
        : m_decomposition(n_domains, node_count, periodic_directions(boundary_types), std::array<idx_t, Dim>{})
        , m_phys_dims 
    {

    }


    */

    std::pair<Boundary, Boundary> get_boundaries(Direction dir) const{

        idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        return m_boundaries[dir_idx];

        Utils::runtime_assert(m_boundaries.first.location == BoundaryLocation::begin, "Invalid boundary order.");
        Utils::runtime_assert(m_boundaries.second.location == BoundaryLocation::end, "Invalid boundary order");

    }

    Boundary get_boundary(Direction dir, BoundaryLocation location) const{

        auto boundaries = get_boundaries(dir);
        if (location == BoundaryLocation::begin){
            return boundaries.first;
        }
        return boundaries.second;
    }

private:
    Decomposition<Dim>                m_decomposition;
    std::array<std::pair<Boundary, Boundary>, Dim> m_boundaries;
//    UniformGrid<Dim>                    m_grid; 

    static std::array<idx_t, Dim> periodic_directions(boundary_array b){

        std::array<idx_t, Dim> ret;
        for (idx_t i = 0; i < Dim; ++i){
            ret[i] = (b[i].first.type == BoundaryType::periodic) && (b[i].second.type == BoundaryType::periodic); 
        }
        return ret;
    }



};

} // namespace JADA
