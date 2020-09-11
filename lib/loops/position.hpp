#pragma once
#include <array>
#include "loops/index_type.hpp"

namespace JADA{

template <idx_t N> using position = std::array<idx_t, N>;
}
