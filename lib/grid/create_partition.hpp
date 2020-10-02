#pragma once

#include "grid/partition.hpp"


namespace JADA{

enum class PartitionId { begin, middle, end };

/*
Partition(std::array<idx_t, Dim> parent_dims,
              Direction              dir,
              idx_t                  begin_idx,
              idx_t                  width
*/

template<idx_t Dim, class Stencil>
auto create_partitions(const Grid<Dim>& grid, Direction dir, Stencil stencil){

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

/*
template<idx_t Dim, class Stencil>
static Partition<Dim> create_partition(const Grid<Dim>& grid, Direction dir, Stencil stencil, PartitionId id){

    idx_t begin_width = stencil.left_halfwidth();
    idx_t end_width = stencil.right_halfwidth();
    idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
    auto grid_dims = grid.dimensions();
    idx_t middle_width = grid_dims[dir_idx] - begin_width - end_width;

    std::array<idx_t, Dim> begin{};

    switch (id)
    {
    case PartitionId::begin:
        return Partition<Dim>(grid_dims, begin, dir, begin_width); 
    case PartitionId::middle:

        
    
    default:
        break;
    }




}
*/


}