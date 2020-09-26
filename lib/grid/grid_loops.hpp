#pragma once
#include <array>
#include <iostream>

#include "grid/grid.hpp"
#include "grid/direction.hpp"
#include "loops/serial_index_loops.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{

template <idx_t Dim, class Method>
inline static index_generator<1> directional_loop(const Grid<Dim>& grid, Direction dir, Method method) {

    const auto dims = grid.dimensions();
    auto begin = std::array<idx_t, Dim>{};
    const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
    
    begin[dir_idx] += method.left_halfwidth();
    
    auto end = dims;
    end[dir_idx] -= method.right_halfwidth();

    Utils::runtime_assert(begin <= end, "Invalid directional loop");


    return serial_index(begin, end, dims);
}

//This is the left "begin" boundary loop
template <idx_t Dim, class Method>
inline static index_generator<1> directional_loop_begin(const Grid<Dim>& grid, Direction dir, Method method) {

    auto dims = grid.dimensions();
    auto begin = std::array<idx_t, Dim>{};
    const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);

    //TODO:
    //maybe shift all but not the Direction index by left halfwidth
    //from begin

    auto end = dims;

    end[dir_idx] = method.left_halfwidth();

    Utils::runtime_assert(begin <= end, "Invalid directional loop");

    return serial_index(begin, end, dims);
}

//this is the "end" boundary loop
template <idx_t Dim, class Method>
inline static index_generator<1> directional_loop_end(const Grid<Dim>& grid, Direction dir, Method method) {

    const auto dims = grid.dimensions();
    auto begin = std::array<idx_t, Dim>{};
    const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);

    begin[dir_idx] = dims[dir_idx] - method.right_halfwidth();

    auto end = dims;


    Utils::runtime_assert(begin <= end, "Invalid directional loop");
    return serial_index(begin, end, dims);
}



}

