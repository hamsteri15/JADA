#pragma once

#include <utility> //std::pair

#include "grid/boundary.hpp"
#include "grid/decomposition.hpp"
#include "grid/direction.hpp"
#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <class GridType> struct Domain {

    static constexpr idx_t Dim = GridType::Dim;

    using boundary_array = std::array<std::pair<Boundary, Boundary>, Dim>;






    Domain(std::array<double, Dim> begin_coord,
           std::array<double, Dim> end_coord,
           std::array<idx_t, Dim>  node_count,
           boundary_array          boundary_types)
        : m_decomposition(1,
                          node_count,
                          periodic_directions(boundary_types),
                          std::array<idx_t, Dim>{})
        , m_begin_coord(begin_coord)
        , m_end_coord(end_coord)
        , m_boundaries(boundary_types) {}


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
    Decomposition<Dim>                             m_decomposition;
    GridType                                       m_grid;
    std::array<double, Dim>                        m_begin_coord;
    std::array<double, Dim>                        m_end_coord;
    std::array<std::pair<Boundary, Boundary>, Dim> m_boundaries;

    //    UniformGrid<Dim>                    m_grid;

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
