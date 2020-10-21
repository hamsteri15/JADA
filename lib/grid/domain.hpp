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

template<idx_t Dim> struct DomainNew{

    using boundary_array = std::array<std::pair<Boundary, Boundary>, Dim>;

    DomainNew(Point<Dim> begin, Point<Dim> end, boundary_array boundaries) : 
    m_begin(begin),
    m_end(end),
    m_boundaries(boundaries)
    {}


private:
    Point<Dim> m_begin;
    Point<Dim> m_end;
    boundary_array m_boundaries;


    

};
/*

static boundary_array create_boundaries(const Domain<Dim>& parent,
                                            Decomposition<Dim> dec,
                                            idx_t              domain_id) {

        auto domain_coords = id_to_domain_coords(dec, domain_id);
        auto domain_counts = dec.get_subdomain_counts();

        boundary_array boundaries;

        for (idx_t i = 0; i < Dim; ++i) {
            Boundary begin;
            begin.location = BoundaryLocation::begin;
            begin.type     = parent.get_boundaries()[i].first.type;

            Boundary end;
            end.location = BoundaryLocation::end;
            end.type     = parent.get_boundaries()[i].second.type;

            if (domain_coords[i] != 0) { begin.type = BoundaryType::processor; }

            if (domain_coords[i] != domain_counts[i] - 1) {
                end.type = BoundaryType::processor;
            }

            boundaries[i] = std::make_pair(begin, end);
        }

        return boundaries;
    }

*/


template <idx_t Dim> struct Domain {

    using boundary_array = std::array<std::pair<Boundary, Boundary>, Dim>;

    Domain(GridDims<Dim> grid_dims, boundary_array boundary_types)
        : m_grid_dims(grid_dims)
        , m_boundaries(boundary_types) {}

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

    boundary_array get_boundaries() const { return m_boundaries; }

    GridDims<Dim> grid_dimensions() const { return m_grid_dims; }

    std::array<idx_t, Dim> periodic_directions() const {
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
