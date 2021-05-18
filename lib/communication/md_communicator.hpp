#pragma once

#include "grid/decomposition.hpp"
#include "grid/neighbours.hpp"
#include "loops/direction.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <size_t N, ConnectivityType CT> class MdCommunicator {

public:
    MdCommunicator() = default;

    MdCommunicator(idx_t id, Decomposition<N> dec)
        : m_id(id)
        , m_dec(dec) {
            Utils::runtime_assert(dec.valid_id(id), "Invalid part id, typically the id exceeds the number of partitions.");
        }

    /*
    MdCommunicator(idx_t id, dimension<N> grid_dims, size_t n_partitions, std::array<bool, N> periodicity) : 
    MdCommunicator(id, Decomposition<N>(grid_dims, n_partitions, periodicity))
    {}
    */


    idx_t id() const { return m_id; }

    static constexpr size_t neighbour_count() { return m_neighbours.count(); }

    static constexpr auto get_directions() { return m_neighbours.get(); }

    bool has_neighbour(direction<N> dir) const {
        return m_dec.get_neighbour(m_id, dir) != NEIGHBOUR_ID_NULL;
    }

    Decomposition<N> get_decomposition() const { return m_dec; }

    size_t get_neighbour(direction<N> dir) const {

        Utils::runtime_assert(has_neighbour(dir),
                              "Trying to get invalid neighbour");
        return size_t(m_dec.get_neighbour(m_id, dir));
    }


    dimension<N> get_global_grid_dimensions() const {
        return m_dec.global_grid_dimensions();
    }

    dimension<N> get_local_grid_dimensions() const {
        return m_dec.local_grid_dimensions(this->id());
    }




protected:

    static constexpr size_t neighbour_idx(direction<N> dir) {
        return size_t(m_neighbours.idx(dir));
    }

private:
    static constexpr Neighbours<N, CT> m_neighbours{};
    idx_t                              m_id;
    Decomposition<N>                   m_dec;
};

} // namespace JADA