#pragma once

#include <utility> //std::pair

#include "grid/boundary_conditions.hpp"
#include "grid/decomposition.hpp"
#include "grid/direction.hpp"
#include "grid/grid_dimensions.hpp"
#include "grid/point.hpp"
#include "loops/index_type.hpp"
#include "loops/unflatten_index.hpp"
#include "utils/runtime_assert.hpp"





namespace JADA {

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

template <size_t Dim> struct Domain {


    Domain(Point<Dim> begin, Point<Dim> end, BoundaryConditions<Dim> bcs)
        : m_begin(begin)
        , m_end(end)
        , m_boundaries(bcs) {}

    BoundaryConditions<Dim> get_boundary_conditions() const { return m_boundaries; }



protected:
private:
    Point<Dim>     m_begin;
    Point<Dim>     m_end;
    BoundaryConditions<Dim> m_boundaries;

};

} // namespace JADA
