#pragma once

#include <hpx/include/lcos.hpp>


#include "containers/md_view.hpp"
#include "grid/neighbours.hpp"
#include "loops/direction.hpp"

namespace JADA {


template <size_t N, class T, ConnectivityType CT> 
struct StructuredDataView {

    using storage_t = std::vector<T>;    
    using halo_storage = std::vector<hpx::shared_future<storage_t>>;

    StructuredDataView(dimension<N> dim,
                       dimension<N> padding,
                       const storage_t&   interior,
                       halo_storage halos,
                       std::vector<direction<N>> halo_dirs)
        : m_dim(dim)
        , m_padding(padding)
        , m_interior(interior)
        , m_halos(halos)
        , m_halo_dirs(halo_dirs) 
        {}


    ///
    ///@brief Get a non-modifyable reference to the element at position pos. This call allows for
    ///       accessing data at the halos such that the input pos can exceed the m_dims
    ///       by min/max m_padding. For example dims = {10, 10} and padding{1,1}, would allow
    ///       positions: {-1, -1} or {11, 11} to be accessed
    ///
    ///@param pos the position of the element to get
    ///@return const T& reference to element at pos
    ///
    const T& at(position<N> pos) const {

        if (in_interior(pos)) {
            auto iview = MdView(m_dim, m_interior);
            return iview[pos];
        }

        return const_halo_element_access(pos);
    }






private:
    static constexpr Neighbours<N, CT> m_neighbours{};

    dimension<N>               m_dim;
    dimension<N>               m_padding;
    const storage_t&              m_interior;
    halo_storage           m_halos;
    std::vector<direction<N>> m_halo_dirs;


    const storage_t& get_halo(direction<N> dir) const {

        size_t idx = 0;
        for (auto d : m_halo_dirs) {
            if (d == dir) {
                return m_halos[idx].get();
            }

            idx++;
        }

        throw std::logic_error("Halo not found.");


    }


    const T& const_halo_element_access(position<N> pos) const{

        const auto dir = get_direction(pos);
        const auto& vdata = get_halo(dir);
        const auto x = pos - halo_begin(dir);
    
        auto dims = compute_halo_dims(m_dim, dir, m_padding);
        auto view = MdView(dims, vdata);

        return view[x];


    }

    static dimension<N> compute_halo_dims(dimension<N> dims,
                            position<N>  dir,
                            dimension<N> padding) {

        dimension<N> h_dims{};

        for (size_t i = 0; i < N; ++i) {
            if (dir[i] != 0) {
                h_dims[i] = padding[i];
            } 
            else {
                h_dims[i] = dims[i];
            }
        }

        return h_dims;
    }

    direction<N> get_direction(position<N> pos) const {


        direction<N> dir{};

        for (size_t i = 0; i < N; ++i) {

            if (pos[i] < 0) {
                dir[i] = -1;
            }

            else if (pos[i] >= idx_t(m_dim[i])) {
                dir[i] = 1;
            }
        }
        return dir;
    }

    ///
    ///@brief Get the beginning position of the halo at direction dir
    ///
    ///@param dir direction of the halo region
    ///@return position<N> beginning position of the halo
    ///
    position<N> halo_begin(direction<N> dir) const {

        position<N> begin{};

        for (size_t i = 0; i < N; ++i){
            if (dir[i] < 0) {
                begin[i] = -idx_t(m_padding[i]);
            }

            if (dir[i] > 0) {
                begin[i] = idx_t(m_dim[i]);
            }
        }
        return begin;
    }


    ///
    ///@brief Checks if a position belongs to interior region (m_data)
    ///
    ///@param pos position to query
    ///@return true if belongs
    ///@return false otherwise
    ///
    bool in_interior(position<N> pos) const {

        return pos.all_positive() && (pos < m_dim);
    }


};

} // namespace JADA


