#pragma once

#include "containers/md_array.hpp"
#include "containers/structured_data_subsets.hpp"
#include "grid/block_neighbours.hpp"

namespace JADA {

template <size_t N, class T> struct StructuredData {

    StructuredData() = default;

    StructuredData(dimension<N> dim, dimension<N> padding)
        : m_dim(dim)
        , m_padding(padding)
        , m_data(dim)
        , m_halos(create_halos(dim, padding)) {}

    const MdArray<N, T>& get_halo(position<N> dir) const {
        size_t idx = static_cast<size_t>(m_neighbours.idx(dir));
        return m_halos.at(idx);
    }

    void put_halo(const MdArray<N, T>& data, position<N> dir) {

        size_t idx = static_cast<size_t>(m_neighbours.idx(dir));
        Utils::runtime_assert(m_halos.at(idx).size() == data.size(),
                              "Halo size mismatch");
        m_halos.at(idx) = data;
    }


    T& access_any(position<N> pos) {

        auto p_idx = which_part(pos);
        if (p_idx == -1) {
            return this->operator[](pos);
        }

        //else do vector arithmetic

        throw std::logic_error("Not implemented");





    }




private:
    static constexpr ConnectivityType       CT = ConnectivityType::Star;
    static constexpr BlockNeighbours<N, CT> m_neighbours{};

    dimension<N>               m_dim;
    dimension<N>               m_padding;
    MdArray<N, T>              m_data;
    std::vector<MdArray<N, T>> m_halos;

    static std::vector<MdArray<N, T>> create_halos(dimension<N> dim,
                                                   dimension<N> padding) {

        //TODO: preallocate and reserve/emplace or make halos an array
        std::vector<MdArray<N, T>> ret;

        for (auto dir : m_neighbours.get()) {

            position<N> extent = halo_extent(dim, position<N>(dir), padding);
            Utils::runtime_assert(extent.all_positive(),
                                  "Negative halo dimension");

            dimension<N> dims{};
            for (size_t i = 0; i < N; ++i) {
                dims[i] = static_cast<size_t>(extent[i]);
            }

            ret.push_back(
                MdArray<N,T>(dims)
            );
        }
        return ret;

    }

    
    idx_t which_part(position<N> pos) const {

        //TODO: make a separate function which returns a direction and the direction should handle
        //      shifts of larger than one, i.e. communications overlapping the next partition

        auto min = position<N>{} - m_padding;
        auto max = m_dim         + m_padding;

        //TODO: get rid off
        Utils::runtime_assert(
            (pos >= min) && (pos < max),
            "Position out of bounds" 
        );


        position<N> dir{};

        for (size_t i = 0; i < N; ++i){

            if (pos[i] < 0) {
                dir[i] = -1;
            }

            else if (pos[i] >= m_dim[i]){
                dir[i] = 1;
            }
            //TODO: get rid off
            else {
                dir[i] = 0;
            }



        }

        return m_neighbours.idx(dir);

    }







};

} // namespace JADA
