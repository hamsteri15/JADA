#pragma once

#include "loops/index_type.hpp"

namespace JADA {

template <idx_t N> using BlockDims = std::array<idx_t, N>;

template <idx_t Dim> struct Block {

    Block() = default;

    explicit Block(BlockDims<Dim> dims, idx_t id_)
        : dimensions(dims), id(id_) {}

    BlockDims<Dim> dimensions;
    idx_t          id;
};











} // namespace JADA