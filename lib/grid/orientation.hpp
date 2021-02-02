#pragma once

#include <cstddef>

#include "loops/index_type.hpp"
#include "loops/position.hpp"

namespace JADA{

struct Orientation{


    explicit constexpr Orientation(size_t id) : m_id(id) {}

    template<size_t N>
    explicit constexpr Orientation(position<N> direction) {
        Utils::runtime_assert(direction.elementwise_sum() == 1, "Invalid direction.");

        auto iter = std::find_if(std::cbegin(direction),
                                 std::cend(direction),
                                 [](idx_t value) { return value == 1; });

        m_id = size_t(std::distance(std::cbegin(direction), iter));

    }


    template<size_t N>
    constexpr position<N> get_direction() const {
        Utils::runtime_assert(m_id < N, "Invalid direction");
        position<N> p{};
        p[m_id] = idx_t(1);
        return p;
    }



    constexpr auto operator<=>(const Orientation&) const = default;


    constexpr size_t id() const {return m_id;}

private:

    size_t m_id;

};

template <size_t N> static constexpr position<N> to_direction(Orientation o) {
    Utils::runtime_assert(o.id() < N, "Invalid direction");
    position<N> p{};
    p[o.id()] = idx_t(1);
    return p;
}
}