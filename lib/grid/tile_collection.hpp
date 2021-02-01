#pragma once

#include <cstddef>
#include <array>

#include "grid/tile.hpp"
#include "grid/orientation.hpp"


namespace JADA{

template <size_t N> struct TileCollection {

    explicit constexpr TileCollection(std::array<Tile, N> tiles)
        : m_tiles(tiles) {}

    constexpr std::array<Tile, N> get_tiles() const { return m_tiles; }

    constexpr std::vector<Tile> get_tiles(Orientation orr) const {

        // Not sure if this is stack allocated or not,
        // consider returning an array or an iterator pair
        std::vector<Tile> ret; ret.reserve(N);

       for (const auto& t : m_tiles){
           if (t.get_orientation() == orr) {
               ret.push_back(t);
           }
       }


        return ret;
    }

    constexpr idx_t barrier_begin(Orientation o) const {
        return std::max_element(
            std::cbegin(m_tiles),
            std::cend(m_tiles),
            [&](const Tile& lhs, const Tile& rhs) {return lhs.barrier_begin(o) < rhs.barrier_begin(o);}
        )->barrier_begin(o);
    }

    constexpr idx_t barrier_end(Orientation o) const {
        return std::max_element(
            std::cbegin(m_tiles),
            std::cend(m_tiles),
            [&](const Tile& lhs, const Tile& rhs) {return lhs.barrier_end(o) < rhs.barrier_end(o);}
        )->barrier_end(o);
    }




private:
    std::array<Tile, N> m_tiles;
};


}