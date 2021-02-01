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
        std::vector<Tile> ret;
        ret.reserve(N);

        std::copy_if(
            std::cbegin(m_tiles),
            std::cend(m_tiles),
            std::begin(ret),
            [&](const Tile& t) { return t.get_orientation() == orr; });

        return ret;
    }

private:
    std::array<Tile, N> m_tiles;
};


}