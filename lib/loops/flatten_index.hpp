#pragma once

#include <array>

#include "utils/runtime_assert.hpp"
#include "utils/can_throw.hpp"

namespace JADA {

constexpr size_t flatten(const std::array<size_t, 1>& dimension,
                         size_t                       i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(i < dimension[0], "Index<0> Out of bounds");

    return i;
}

constexpr size_t
flatten(const std::array<size_t, 2>& dimension, size_t j, size_t i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(j < dimension[0], "Index 0 Out of bounds");
    Utils::runtime_assert(i < dimension[1], "Index 1 Out of bounds");

    return i + dimension[1] * j;
}

constexpr size_t flatten(const std::array<size_t, 3>& dimension,
                         size_t                       k,
                         size_t                       j,
                         size_t                       i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(k < dimension[0], "Index 0 Out of bounds");
    Utils::runtime_assert(j < dimension[1], "Index 1 Out of bounds");
    Utils::runtime_assert(i < dimension[2], "Index 2 Out of bounds");

    return i + j * dimension[2] + k * dimension[1] * dimension[2];
}

constexpr size_t flatten(const std::array<size_t, 4>& dimension,
                         size_t                       l,
                         size_t                       k,
                         size_t                       j,
                         size_t                       i) noexcept(Utils::can_throw) {

    Utils::runtime_assert(l < dimension[0], "Index 0 Out of bounds");
    Utils::runtime_assert(k < dimension[1], "Index 1 Out of bounds");
    Utils::runtime_assert(j < dimension[2], "Index 2 Out of bounds");
    Utils::runtime_assert(i < dimension[3], "Index 3 Out of bounds");

    return i + j * dimension[3] + k * dimension[3] * dimension[2] +
           l * dimension[3] * dimension[2] * dimension[1];
}

} // namespace Math
