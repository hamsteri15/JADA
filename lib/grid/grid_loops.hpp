#pragma once
#include <array>

#include "grid/grid.hpp"
#include "grid/direction.hpp"
#include "loops/serial_index_loops.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{

template <idx_t Dim, class Method>
inline static index_generator<1> grid_directional_loop(const Grid<Dim>& grid, Direction dir, Method method) {

    const auto dims = grid.dimensions();
    auto begin = std::array<idx_t, Dim>{};
    const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
    
    begin[dir_idx] += method.left_halfwidth();
    
    auto end = dims;
    end[dir_idx] -= method.right_halfwidth();

    Utils::runtime_assert(begin <= end, "Invalid directional loop");


    return serial_index(begin, end, dims);
}

}

