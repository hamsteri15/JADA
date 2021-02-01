#pragma once

#include <array>
#include "loops/index_type.hpp"
#include "loops/position.hpp"


namespace JADA{


struct Orientation{


    explicit constexpr Orientation(size_t id) : m_id(id) {}

    template<size_t N>
    position<N> get_direction() const {
        Utils::runtime_assert(m_id < N, "Invalid orientation");
        position<N> p{};
        p[m_id] = idx_t(1);
        return p;
    }

    constexpr auto operator<=>(const Orientation&) const = default;




private:

    size_t m_id;

};


struct Tile {

    constexpr Tile() = default;

    constexpr Tile(idx_t min, idx_t max, Orientation orr)
        : m_min(min)
        , m_max(max)
        , m_orr(orr) {}

    constexpr idx_t  get_min() const { return m_min; }
    constexpr idx_t  get_max() const { return m_max; }
    constexpr size_t get_width() const { return size_t(m_max - m_min + 1); }
    constexpr Orientation get_orientation() const { return m_orr; }


    


    constexpr idx_t barrier_end() const {
        return std::max(idx_t(0), get_max());
    }

    constexpr idx_t barrier_begin() const {
        return std::max(idx_t(0), -get_min());
    }


private:
    idx_t  m_min, m_max;
    Orientation m_orr;
};





template <size_t N> struct Tiles {

    explicit constexpr Tiles(std::array<Tile, N> tiles)
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