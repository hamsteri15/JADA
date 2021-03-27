#pragma once

#include "containers/md_array.hpp"
#include "grid/neighbours.hpp"
#include "loops/direction.hpp"

namespace JADA {

template <size_t N, class T> struct StructuredData {

    using storage_t = MdArray<N, T, StorageOrder::RowMajor>;


    StructuredData() = default;

    StructuredData(dimension<N> dim, dimension<N> padding)
        : m_dim(dim)
        , m_padding(padding)
        , m_data(dim)
        , m_halos(create_halos(dim, padding)) {}


    void put_halo(const storage_t& data, direction<N> dir) {

        size_t idx = static_cast<size_t>(m_neighbours.idx(dir));
        Utils::runtime_assert(m_halos.at(idx).size() == data.size(),
                              "Halo size mismatch");
        m_halos.at(idx) = data;
    }


    const MdArray<N, T> get_interior() const {
        return m_data;
    }


    void set_all(T val) {
        m_data.set_all(val);
        for (auto& p : m_halos) {
            p.set_all(val);
        }
    }


    T& access_any(position<N> pos) {

        auto p_idx = which_part(pos);
        if (p_idx == -1) {
            return m_data[pos];
        }

        auto begin = halo_begin(which_dir(pos));
        auto x = pos - begin;

        auto& part = m_halos[size_t(p_idx)];
        return part[x];


    }




private:
    static constexpr ConnectivityType       CT = ConnectivityType::Star;
    static constexpr Neighbours<N, CT> m_neighbours{};

    dimension<N>               m_dim;
    dimension<N>               m_padding;
    storage_t              m_data;
    std::vector<storage_t> m_halos;

    
    
    static std::vector<storage_t> create_halos(dimension<N> dim,
                                                   dimension<N> padding) {

        std::vector<storage_t> ret; 
        ret.reserve(m_neighbours.count()); 

        for (auto dir : m_neighbours.get()) {

            ret.emplace_back(
                MdArray<N,T>(compute_halo_dims(dim, dir, padding))
            );            

        }
        return ret;

    }

    position<N> halo_begin(direction<N> dir) const {

        position<N> a{};

        for (size_t i = 0; i < N; ++i){
            if (dir[i] < 0) {
                a[i] = -idx_t(m_padding[i]);
            }

            if (dir[i] > 0) {
                a[i] = idx_t(m_dim[i]);
            }
        }
        return a;
    }



    idx_t which_part(position<N> pos) const {

        auto dir = which_dir(pos);

        return m_neighbours.idx(dir);

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





    direction<N> which_dir(position<N> pos) const {

        auto min = -position<N>(m_padding);
        auto max = position<N>(m_dim + m_padding);

        // TODO: get rid off
        Utils::runtime_assert((pos >= min) && (pos < max),
                              "Position out of bounds");

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


};

} // namespace JADA
