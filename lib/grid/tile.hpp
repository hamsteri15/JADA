#pragma once

#include <array>
#include "loops/index_type.hpp"



namespace JADA{


struct Tile {

    constexpr Tile(idx_t min, idx_t max, size_t orr)
        : m_min(min)
        , m_max(max)
        , m_orr(orr) {}

    constexpr idx_t  get_min() const { return m_min; }
    constexpr idx_t  get_max() const { return m_max; }
    constexpr size_t get_width() const { return size_t(m_max - m_min + 1); }
    constexpr size_t get_orientation() const { return m_orr; }


    


    constexpr idx_t barrier_end() const {
        return std::max(idx_t(0), get_max());
    }

    constexpr idx_t barrier_begin() const {
        return std::max(idx_t(0), -get_min());
    }


private:
    idx_t  m_min, m_max;
    size_t m_orr;
};

template<size_t N>
struct Tiles{

    explicit constexpr Tiles(std::array<Tile, N> tiles) : m_tiles(tiles) {



    }

    constexpr std::array<Tile, N> get_tiles() const {return m_tiles;}


    constexpr auto get_tiles(size_t orientation) const {

        constexpr size_t count = unique(orientation);

        std::array<Tile, count> ret;
        return ret;


    }
    





private:

    std::array<Tile, N> m_tiles;


    constexpr size_t unique(size_t orientation) const {
        size_t count = 0;
        for (const auto& t : m_tiles){
            if (t.get_orientation() == orientation){
                count++;
            }
        }
        return count;
    }



};


}