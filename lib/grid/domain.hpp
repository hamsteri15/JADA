#pragma once

#include <utility> //std::pair

#include "grid/boundary.hpp"
#include "grid/decomposition.hpp"
#include "grid/direction.hpp"
#include "grid/grid_dimensions.hpp"
#include "grid/point.hpp"
#include "loops/index_type.hpp"
#include "loops/unflatten_index.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <idx_t Dim> struct Domain {

    using boundary_array = std::array<std::pair<Boundary, Boundary>, Dim>;

    Domain(
           GridDims<Dim>  grid_dims,
           boundary_array boundary_types)
        : m_grid_dims(grid_dims)
        , m_boundaries(boundary_types) {
    }

    std::pair<Boundary, Boundary> get_boundaries(Direction dir) const {

        idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);

        Utils::runtime_assert(m_boundaries[dir_idx].first.location ==
                                  BoundaryLocation::begin,
                              "Invalid boundary order.");
        Utils::runtime_assert(m_boundaries[dir_idx].second.location ==
                                  BoundaryLocation::end,
                              "Invalid boundary order");

        return m_boundaries[dir_idx];

        
    }

    Boundary get_boundary(Direction dir, BoundaryLocation location) const {

        auto boundaries = get_boundaries(dir);
        if (location == BoundaryLocation::begin) { return boundaries.first; }
        return boundaries.second;
    }

    boundary_array get_boundaries() const {
        return m_boundaries;
    }

    GridDims<Dim> grid_dimensions() const {
        return m_grid_dims;
    }


    std::array<idx_t, Dim> periodic_directions() const{
        return periodic_directions(m_boundaries);
    }

protected:



private:
    GridDims<Dim>                                  m_grid_dims;
    std::array<std::pair<Boundary, Boundary>, Dim> m_boundaries;


    static std::array<idx_t, Dim> periodic_directions(boundary_array b) {

        std::array<idx_t, Dim> ret;
        for (idx_t i = 0; i < Dim; ++i) {
            ret[i] = (b[i].first.type == BoundaryType::periodic) &&
                     (b[i].second.type == BoundaryType::periodic);
        }
        return ret;
    }
};










} // namespace JADA
