#pragma once

#include "loops/position.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA{


template<size_t L>
struct direction : public position<L> {

    using element_t = position<L>::element_t;


    inline constexpr direction() = default;

    inline constexpr direction(position<L> pos) : position<L>(pos) {}


    inline constexpr direction(std::array<element_t, L> arr) : position<L>(arr) {

        Utils::runtime_assert(
            (this->min() > -2) && (this->max() < 2),
            "Direction elements have to be either -1, 0 or 1."
        );

    } 


    constexpr direction(std::initializer_list<element_t> list) : position<L>(list) {
        Utils::runtime_assert(
            (this->min() > -2) && (this->max() < 2),
            "Direction elements have to be either -1, 0 or 1."
        );
    }


};

}


