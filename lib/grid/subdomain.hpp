#pragma once

#include "grid/decomposition.hpp"
#include "grid/domain.hpp"
#include "loops/unflatten_index.hpp"

namespace JADA {

template <idx_t Dim> struct SubDomain : public Domain<Dim> {

    using base_type      = Domain<Dim>;
    using boundary_array = typename base_type::boundary_array;

    SubDomain(const Domain<Dim>& parent,
              Decomposition<Dim> dec,
              idx_t              domain_id)
        : Domain<Dim>(compute_grid_dims(dec, domain_id),
                      create_boundaries(parent, dec, domain_id))
        , m_dec(dec)
        , m_id(domain_id) {}




    SubDomain(const Domain<Dim>& parent, idx_t n_subdomains, idx_t domain_id)
        : SubDomain(
              parent, create_decomposition(parent, n_subdomains), domain_id) {}

private:
    Decomposition<Dim> m_dec;
    idx_t              m_id;

    static Decomposition<Dim> create_decomposition(const Domain<Dim>& parent,
                                                   idx_t n_subdomains) {

        static constexpr GridDims<Dim> barriers = GridDims<Dim>{}; // set to
                                                                   // zero
        return Decomposition<Dim>(n_subdomains,
                                  parent.grid_dimensions(),
                                  parent.periodic_directions(),
                                  barriers);
    }

    static std::array<idx_t, Dim> id_to_domain_coords(Decomposition<Dim> dec,
                                                      idx_t domain_id) {
        return unflatten(dec.get_subdomain_counts(), domain_id);
    }

    static GridDims<Dim> compute_grid_dims(Decomposition<Dim> dec,
                                           idx_t              domain_id) {

        auto domain_coords = id_to_domain_coords(dec, domain_id);
        return dec.get_local_dimensions(domain_coords);
    }

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
};

} // namespace JADA