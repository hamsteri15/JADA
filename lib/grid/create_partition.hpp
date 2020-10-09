#pragma once

#include "grid/partition.hpp"


namespace JADA{



template<class GridType, class Stencil>
auto create_partitions(const GridType& grid, Direction dir, Stencil stencil){

    static constexpr idx_t Dim = GridType::Dim;

    idx_t begin_begin = 0;
    idx_t middle_begin = stencil.left_halfwidth();
    idx_t end_begin = grid.dimension(dir) - stencil.right_halfwidth();

    idx_t begin_width = stencil.left_halfwidth();
    idx_t middle_width = grid.dimension(dir) - stencil.left_halfwidth() - stencil.right_halfwidth();
    idx_t end_width = stencil.right_halfwidth();

    return std::make_tuple(
        Partition<Dim>(grid.dimensions(), dir, begin_begin, begin_width),
        Partition<Dim>(grid.dimensions(), dir, middle_begin, middle_width),
        Partition<Dim>(grid.dimensions(), dir, end_begin, end_width)
    );


}



}