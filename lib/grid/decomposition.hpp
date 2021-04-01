#pragma once

#include <array>
#include <vector>

#include "grid/split.hpp"
#include "grid/splitting_policy_min_diff.hpp"
#include "loops/flatten_index.hpp"
#include "loops/unflatten_index.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

static constexpr idx_t NEIGHBOUR_ID_NULL = -42;

template <size_t N> struct Decomposition {

    Decomposition() {

        for (size_t i = 0; i < N; ++i){
            m_grid_dims[i] = 0;
            m_dec_dims[i] = 0;
            m_periodicity[i] = false;
        }

    }

    ///
    ///@brief Construct a decomposition from specified subdomain grid.
    ///
    ///@param grid_dims Global grid dimensions.
    ///@param dec_dims  Subdomain grid dimensions.
    ///@param periodicity Dimensions specific periodicity.
    ///
    Decomposition(dimension<N>        grid_dims,
                  dimension<N>        dec_dims,
                  std::array<bool, N> periodicity)
        : m_grid_dims(grid_dims)
        , m_dec_dims(dec_dims)
        , m_periodicity(periodicity) {

        Utils::runtime_assert(std::equal(std::begin(dec_dims),
                                         std::end(dec_dims),
                                         std::begin(grid_dims),
                                         std::less_equal{}),
                              "Invalid decomposition");
    }

    ///
    ///@brief Construct a decomposition from number of subdomains.
    ///
    ///@param grid_dims Global grid dimensions.
    ///@param n_domains Number of subdomains.
    ///@param periodicity Dimension specific periodicity.
    ///
    Decomposition(dimension<N>        grid_dims,
                  size_t              n_domains,
                  std::array<bool, N> periodicity)
        : Decomposition(grid_dims, split(grid_dims, n_domains), periodicity) {}

    ///
    ///@brief Given a domain id returns the grid offset wrt. position zero of
    ///       the global grid.
    ///       The offset can be used to convert local grid indices to global
    ///       indices by simple addition: global = local + offset
    ///
    ///@param domain_id Id to query the offset for.
    ///@return position<N> The offset.
    ///
    position<N> get_offset(idx_t domain_id) const {

        Utils::runtime_assert(valid_id(domain_id), "Invalid domain id");

        auto        d_coords = get_domain_coords(domain_id);
        position<N> offset;

        for (size_t i = 0; i < N; ++i) {
            offset[i] = idx_t(m_grid_dims[i] / m_dec_dims[i]) * d_coords[i];
        }
        return offset;
    }

    ///
    ///@brief Given a domain id returns the local grid dimensions.
    ///
    ///@param domain_id Id to query the dimensions for.
    ///@return dimension<N> Local grid dimensions.
    ///
    dimension<N> local_grid_dimensions(idx_t domain_id) const {

        Utils::runtime_assert(valid_id(domain_id), "Invalid domain id");

        auto local_dims = m_grid_dims / m_dec_dims;
        auto remainder  = m_grid_dims % m_dec_dims;
        auto d_coords   = get_domain_coords(domain_id);

        // this handles uneven node counts by adding the reminder to the last
        // domain in the grid
        for (size_t i = 0; i < N; ++i) {
            if (d_coords[i] == idx_t(m_dec_dims[i] - 1)) {
                local_dims[i] += remainder[i];
            }
        }

        Utils::runtime_assert(valid_local_dims(local_dims),
                              "Invalid local dimensions");
        return local_dims;
    }

    ///
    ///@brief Given a domain id and an offset (direction) in the domain grid
    ///       returns the neighbour id.
    ///       For instance in a two dimensional grid an offset of {1,0} would
    ///       return the "north" neighbour id of 'domain_id'. For border domains
    ///       without periodicity returns a NEIGHBOUR_ID_NULL.
    ///
    ///@param domain_id Id to query the neighbour for.
    ///@param offset The offset wrt. to domain id to get the neighbour id.
    ///@return idx_t The neigbhouring domain id.
    ///
    idx_t get_neighbour(idx_t domain_id, position<N> offset) const {

        Utils::runtime_assert(valid_id(domain_id), "Invalid domain id");

        
        Utils::runtime_assert(std::equal(std::begin(offset),
                                         std::end(offset),
                                         std::begin(m_dec_dims),
                                         [](idx_t o, size_t d) {
                                             return std::abs(o) <= idx_t(d);
                                         }),
                              "Offset out of bounds.");

        

        auto n_coords = get_domain_coords(domain_id) + offset;

        for (size_t i = 0; i < N; ++i) {

            if (n_coords[i] < 0) {

                if (m_periodicity[i]) {
                    n_coords[i] += idx_t(m_dec_dims[i]);
                } else {
                    return NEIGHBOUR_ID_NULL;
                }
            }

            if (n_coords[i] >= idx_t(m_dec_dims[i])) {

                if (m_periodicity[i]) {
                    n_coords[i] -= idx_t(m_dec_dims[i]);
                } else {
                    return NEIGHBOUR_ID_NULL;
                }
            }
        }
        return get_domain_id(n_coords);
    }

    ///
    ///@brief Get the decomposition dimensions.
    ///
    ///@return dimension<N> Dimensions of the subdomain grid.
    ///
    dimension<N> dimensions() const { return m_dec_dims; }

    ///
    ///@brief Get the global grid point count
    ///
    ///@return dimension<N> The global grid point count
    ///
    dimension<N> global_grid_dimensions() const { return m_grid_dims; }

    ///
    ///@brief Checks that the input id is valid.
    ///
    ///@param domain_id The id to validate.
    ///@return true If valid.
    ///@return false If invalid.
    ///
    bool valid_id(idx_t domain_id) const {
        return (domain_id >= 0) &&
               (domain_id < idx_t(m_dec_dims.elementwise_product()));
    }


private:
    dimension<N>        m_grid_dims;
    dimension<N>        m_dec_dims;
    std::array<bool, N> m_periodicity;

    ///
    ///@brief Converts a domain id to a position in the subdomain grid.
    ///
    ///@param domain_id Id to convert.
    ///@return position<N> Subdomain position.
    ///
    position<N> get_domain_coords(idx_t domain_id) const {
        return unflatten<N, StorageOrder::RowMajor>(m_dec_dims, domain_id);
    }

    ///
    ///@brief Converts a domain coordinate to domain id.
    ///
    ///@param coord Coordinate to convert.
    ///@return idx_t The domain id.
    ///
    idx_t get_domain_id(position<N> coord) const {
        return flatten<N, StorageOrder::RowMajor>(m_dec_dims, coord);
    }


    ///
    ///@brief Checks that the input local grid dimensions are valid.
    ///
    ///@param local_grid_dims The local grid dimensions to validate.
    ///@return true If the grid dimensions are valid.
    ///@return false If the grid dimensions are invalid.
    ///
    bool valid_local_dims(dimension<N> local_grid_dims) const {
        return std::equal(std::begin(local_grid_dims),
                          std::end(local_grid_dims),
                          std::begin(m_grid_dims),
                          std::less_equal{});
    }
};


} // namespace JADA