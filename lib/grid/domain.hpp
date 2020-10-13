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

    Domain(Point<Dim>     begin_coord,
           Point<Dim>     end_coord,
           boundary_array boundary_types)
        : m_begin(begin_coord)
        , m_end(end_coord)
        , m_boundaries(boundary_types) {
        Utils::runtime_assert(begin_coord <= end_coord,
                              "The domain bounds must be such that the begin "
                              "coord is smaller than the end coord.");
    }

    std::pair<Boundary, Boundary> get_boundaries(Direction dir) const {

        idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        return m_boundaries[dir_idx];

        Utils::runtime_assert(m_boundaries.first.location ==
                                  BoundaryLocation::begin,
                              "Invalid boundary order.");
        Utils::runtime_assert(m_boundaries.second.location ==
                                  BoundaryLocation::end,
                              "Invalid boundary order");
    }

    Boundary get_boundary(Direction dir, BoundaryLocation location) const {

        auto boundaries = get_boundaries(dir);
        if (location == BoundaryLocation::begin) { return boundaries.first; }
        return boundaries.second;
    }

private:
    Point<Dim>                                     m_begin;
    Point<Dim>                                     m_end;
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
