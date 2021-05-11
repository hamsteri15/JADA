#pragma once

#include <utility> //std::pair
#include "grid/neighbours.hpp"
#include "loops/direction.hpp"

namespace JADA {

template <size_t N, ConnectivityType CT> 
struct DimensionHandle {

    static constexpr Neighbours<N, CT> m_neighbours{};

    DimensionHandle() = default;


    DimensionHandle(dimension<N> inner_dims) : 
    m_inner_dims(inner_dims),
    m_outer_dims(inner_dims)
    {}


    DimensionHandle(dimension<N> inner_dims, dimension<N> outer_dims) : 
    m_inner_dims(inner_dims),
    m_outer_dims(outer_dims)
    {}



    dimension<N> get_padding() const {
        return (m_outer_dims - m_inner_dims) / 2;
    }

    std::pair<size_t, size_t> pos_to_access(position<N> pos) const {

        Utils::runtime_assert(in_outer(pos),
                              "pos_to_access index out of bounds");

        if (in_inner(pos)) {
            return std::make_pair<size_t, size_t>(
                0,
                size_t(flatten<N, StorageOrder::RowMajor>(m_inner_dims, pos)));
        }


        auto dir = get_direction(pos);
        const auto halo_idx = m_neighbours.idx(dir);
        const auto x = pos - halo_begin(dir);
        const auto halo_dims = compute_halo_dims(
            m_inner_dims,
            dir,
            get_padding()
        );

        return std::make_pair<size_t, size_t>(
            halo_idx + 1,
            size_t(flatten<N, StorageOrder::RowMajor>(halo_dims, x))
        );

    }



    bool in_inner(position<N> pos) const {
        return pos.all_positive() && (pos < m_inner_dims);
    }


    bool in_outer(position<N> pos) const {
        return pos.all_positive() && (pos < m_outer_dims);
    }



    direction<N> get_direction(position<N> pos) const {

        direction<N> dir{};

        for (size_t i = 0; i < N; ++i) {

            if (pos[i] < 0) {
                dir[i] = -1;
            }

            else if (pos[i] >= idx_t(m_inner_dims[i])) {
                dir[i] = 1;
            }
        }
        return dir;
    }

    static dimension<N> compute_halo_dims(dimension<N> dims,
                                          position<N>  dir,
                                          dimension<N> padding) {

        dimension<N> h_dims{};

        for (size_t i = 0; i < N; ++i) {
            if (dir[i] != 0) {
                h_dims[i] = padding[i];
            } else {
                h_dims[i] = dims[i];
            }
        }

        return h_dims;
    }




private:

    dimension<N> m_inner_dims;
    dimension<N> m_outer_dims;



    ///
    ///@brief Get the beginning position of the halo at direction dir
    ///
    ///@param dir direction of the halo region
    ///@return position<N> beginning position of the halo
    ///
    position<N> halo_begin(direction<N> dir) const {

        position<N> begin{};

        auto padding = get_padding();

        for (size_t i = 0; i < N; ++i){
            if (dir[i] < 0) {
                begin[i] = -idx_t(padding[i]);
            }

            if (dir[i] > 0) {
                begin[i] = idx_t(m_inner_dims[i]);
            }
        }
        return begin;
    }

};

} // namespace JADA
