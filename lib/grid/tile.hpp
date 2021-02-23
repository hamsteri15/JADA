#pragma once

#include <utility> //pair

#include "grid/orientation.hpp"
#include "loops/index_type.hpp"
#include "loops/position.hpp"
#include "utils/constexpr_functions.hpp"

namespace JADA {

struct Tile {

    constexpr Tile() = default;

    constexpr Tile(idx_t min, idx_t max, Orientation orr)
        : m_min(min)
        , m_max(max)
        , m_orr(orr) {
        Utils::runtime_assert(max > min, "Tile max has to be larger than min");
    }


    constexpr size_t neighbour_count(Orientation o) const {
        return size_t(get_max(o) - get_min(o));
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

    constexpr idx_t barrier_end(Orientation o) const {
        return std::max(idx_t(0), get_max(o));
    }

    constexpr idx_t barrier_begin(Orientation o) const {
        return std::max(idx_t(0), -get_min(o));
    }

    // Always centered at 0 for now
    constexpr idx_t get_center() const { return 0; }

private:
    idx_t       m_min, m_max;
    Orientation m_orr;
};

} // namespace JADA