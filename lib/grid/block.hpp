#pragma once

#include "loops/index_type.hpp"

namespace JADA {

template <idx_t N> using BlockDims = std::array<idx_t, N>;

template <idx_t Dim> struct Block {

    Block() = default;

    explicit Block(BlockDims<Dim> dims)
        : dimensions(dims)
        , parent_dimensions(BlockDims<Dim>{})
        , level(0)
        , id(0)
        , n_siblings(0) {}

    BlockDims<Dim> dimensions;
    BlockDims<Dim> parent_dimensions;
    idx_t          level;
    idx_t          id;
    idx_t          n_siblings; // number of blocks on the same level
};











} // namespace JADA