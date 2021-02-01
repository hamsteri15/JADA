#pragma once

#include <cstddef>

#include "loops/index_type.hpp"
#include "loops/position.hpp"

namespace JADA{

struct Orientation{


    explicit constexpr Orientation(size_t id) : m_id(id) {}

    template<size_t N>
    constexpr position<N> get_direction() const {
        Utils::runtime_assert(m_id < N, "Invalid orientation");
        position<N> p{};
        p[m_id] = idx_t(1);
        return p;
    }

    constexpr auto operator<=>(const Orientation&) const = default;


private:

    size_t m_id;

};


}