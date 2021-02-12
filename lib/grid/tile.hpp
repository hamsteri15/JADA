#pragma once

#include <utility> //pair

#include "loops/index_type.hpp"
#include "loops/position.hpp"
#include "grid/orientation.hpp"
#include "utils/constexpr_functions.hpp"

namespace JADA{

struct Tile {

    constexpr Tile() = default;

    constexpr Tile(idx_t min, idx_t max, Orientation orr)
        : m_min(min)
        , m_max(max)
        , m_orr(orr) {
            Utils::runtime_assert(max > min, "Tile max has to be larger than min");
        }

    constexpr idx_t get_min(Orientation o) const {
        if (o == m_orr) { return m_min; }
        return 0;
    }
    constexpr idx_t get_max(Orientation o) const {
        if (o == m_orr) { return m_max; }
        return 0;
    }
    constexpr size_t get_width(Orientation o) const {
        return size_t(get_max(o) - get_min(o) + 1);
    }

    constexpr Orientation get_orientation() const { return m_orr; }


    constexpr idx_t barrier_end(Orientation o) const{
        return std::max(idx_t(0), get_max(o));
    }

    constexpr idx_t barrier_begin(Orientation o) const{
        return std::max(idx_t(0), -get_min(o));
    }



private:
    idx_t       m_min, m_max;
    Orientation m_orr;
};

//Total number of boundary grid points influenced by the tile
static constexpr idx_t boundary_width(Tile t, Orientation orr) {

    //auto min = t.get_min(orr);
    //auto max = t.get_max(orr);
    idx_t ngc = t.barrier_begin(orr) + t.barrier_end(orr); 
    return ngc; //+something

}


/*
static constexpr std::pair<idx_t, idx_t> owner_neighbour_width_end(Tile t, Orientation orr) {

    auto total = boundary_width_end(t, orr);
    idx_t n_width = t.barrier_end(orr);
    idx_t o_width = idx_t(total) - n_width;



    return {o_width, n_width};
}
*/









}