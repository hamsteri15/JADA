#pragma once

#include <array>
#include <numeric> //std::accumulate
#include "utils/runtime_assert.hpp"
#include "utils/can_throw.hpp"

namespace JADA {

template <typename INT>
constexpr std::array<INT, 1> unflatten(const std::array<INT, 1>& dimension,
                                       INT idx) noexcept(Utils::can_throw) {

#ifdef DEBUG
    INT total = std::accumulate(dimension.begin(), dimension.end(), INT(1), std::multiplies<INT>{});
    Utils::runtime_assert(idx < total, "Index unflatten out of bounds.");
#endif

    return {idx};
}

template <typename INT>
constexpr std::array<INT, 2> unflatten(const std::array<INT, 2>& dimension,
                                       INT idx) noexcept(Utils::can_throw) {

#ifdef DEBUG
    INT total = std::accumulate(dimension.begin(), dimension.end(), INT(1), std::multiplies<INT>{});
    Utils::runtime_assert(idx < total, "Index unflatten out of bounds.");
#endif

    INT j = idx / dimension[1];
    INT i = idx % dimension[1];

    return {j, i};
}

template <typename INT>
constexpr std::array<INT, 3> unflatten(const std::array<INT, 3>& dimension,
                                       INT idx) noexcept(Utils::can_throw) {

#ifdef DEBUG
    INT total = std::accumulate(dimension.begin(), dimension.end(), INT(1), std::multiplies<INT>{});
    Utils::runtime_assert(idx < total, "Index unflatten out of bounds.");
#endif

    INT k    = idx / (dimension[1] * dimension[2]);
    INT temp = idx - (k * dimension[1] * dimension[2]);
    INT j    = temp / dimension[2];
    INT i    = temp % dimension[2];

    return {k, j, i};
}

} // namespace JADA