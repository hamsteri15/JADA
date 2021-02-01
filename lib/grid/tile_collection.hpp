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
        // consider returning an array somehow
        std::vector<Tile> ret; ret.reserve(N);

       for (const auto& t : m_tiles){
           if (t.get_orientation() == orr) {
               ret.push_back(t);
           }
       }


        return ret;
    }


    constexpr std::vector<Tile> get_tiles(position<N> direction){
        return get_tiles(Orientation(direction));
    }


    /*
    constexpr idx_t barrier_end(position<N> direction) const {

    }
    constexpr idx_t barrier_end() const {
        return std::max(idx_t(0), get_max());
    }

    constexpr idx_t barrier_begin() const {
        return std::max(idx_t(0), -get_min());
    }
    */



private:
    std::array<Tile, N> m_tiles;
};


}