#pragma once

#include "loops/index_type.hpp"
#include <array>

namespace JADA {

template <idx_t N> using GridDims = std::array<idx_t, N>;
}