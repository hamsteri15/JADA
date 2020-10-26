#pragma once

#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <idx_t N> using BlockDensity = std::array<idx_t, N>;

template <idx_t Dim> struct Block {

    Block() = default;

    explicit Block(BlockDensity<Dim> density_, idx_t id_)
        : density(density_), id(id_) {
            Utils::runtime_assert(

            std::all_of(density.begin(), density.end(), [](int i) {return i > 0;}),
            "Trying to create a block with a zero node count."
            );
        }

    BlockDensity<Dim> density;
    idx_t          id;
};











} // namespace JADA