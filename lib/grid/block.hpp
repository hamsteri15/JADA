#pragma once

#include "grid/point.hpp"
#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <idx_t N> using BlockDensity    = std::array<idx_t, N>;
template <idx_t N> using BlockDimensions = std::array<double, N>;

template <idx_t Dim> struct Block {

    Block() = default;

    explicit Block(BlockDensity<Dim> density_,
                   Point<Dim>        p0_,
                   Point<Dim>        p1_,
                   int             id_)
        : density(density_)
        , p0(p0_)
        , p1(p1_)
        , id(id_) {

        Utils::runtime_assert(
            std::all_of(
                density.begin(), density.end(), [](int i) { return i > 0; }),
            "Trying to create a block with a zero node count.");

        Utils::runtime_assert(p0 < p1, "Point p0 must be smaller than p1");




    }

    BlockDensity<Dim> density;
    Point<Dim>        p0;
    Point<Dim>        p1;
    int             id;
};

} // namespace JADA