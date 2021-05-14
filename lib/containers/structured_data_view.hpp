#pragma once

#include "containers/md_view.hpp"
#include "grid/neighbours.hpp"
#include "loops/direction.hpp"
#include "containers/dimension_handle.hpp"

namespace JADA {


template <size_t N, class T, ConnectivityType CT> 
struct StructuredDataView {

    using storage_t = std::vector<T>;    
    using halo_storage = std::vector<hpx::shared_future<storage_t>>;

    StructuredDataView(dimension<N> dim,
                       dimension<N> padding,
                       storage_t&   interior,
                       halo_storage halos)
        : m_dim(dim)
        , m_padding(padding)
        , m_interior(interior)
        , m_halos(halos) {}









private:
    static constexpr Neighbours<N, CT> m_neighbours{};

    dimension<N>               m_dim;
    dimension<N>               m_padding;
    storage_t&              m_interior;
    halo_storage           m_halos;





    /*

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

    T& halo_element_access(position<N> pos) {

        const auto dir = get_direction(pos);
        const auto part_idx = m_neighbours.idx(dir);
        const auto x = pos - halo_begin(dir);

        return m_halos[size_t(part_idx)][x];

    }

    const T& const_halo_element_access(position<N> pos) const{

        const auto dir = get_direction(pos);
        const auto part_idx = m_neighbours.idx(dir);
        const auto x = pos - halo_begin(dir);

        return m_halos[size_t(part_idx)][x];

    }

    ///
    ///@brief Creates the galo buffers based on the dimensions and padding.
    ///
    ///@param dim dimensions of the "internal" data
    ///@param padding external padding to the internal data
    ///@return std::vector<storage_t> vector of halos in correct order
    ///
    static std::vector<storage_t> create_halos(dimension<N> dim,
                                                   dimension<N> padding) {

        std::vector<storage_t> ret; 
        ret.reserve(m_neighbours.count()); 

        for (direction<N> dir : m_neighbours.get()) {

            ret.emplace_back(
                //MdArray<N,T>(halo_dims(dim, padding, dir))
                MdArray<N,T>(compute_halo_dims(dim, dir, padding))
            );            

        }
        return ret;

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

    */

};

} // namespace JADA


