#pragma once

#include <utility> //std::pair

#include "grid/boundary.hpp"
#include "grid/direction.hpp"
#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <idx_t Dim> struct Domain {

    Domain(std::array<double, Dim>           phys_dims,
           std::array<idx_t, Dim>            node_count,
           std::array<std::pair<Boundary, Boundary>, Dim> boundary_types)
        : m_phys_dims(phys_dims)
        , m_node_count(node_count)
        , m_boundaries(boundary_types) {}



    std::pair<Boundary, Boundary> get_boundaries(Direction dir) const{

        idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        return m_boundaries[dir_idx];

        Utils::runtime_assert(m_boundaries.first.location == BoundaryLocation::begin);
        Utils::runtime_assert(m_boundaries.second.location == BoundaryLocation::end);

    }

    Boundary get_boundary(Direction dir, BoundaryLocation location) const{

        auto boundaries = get_boundaries(dir);
        if (location == BoundaryLocation::begin){
            return boundaries.first;
        }
        return boundaries.second;
    }

private:
    std::array<double, Dim>           m_phys_dims;
    std::array<idx_t, Dim>            m_node_count;
    std::array<std::pair<Boundary, Boundary>, Dim> m_boundaries;


};

} // namespace JADA
