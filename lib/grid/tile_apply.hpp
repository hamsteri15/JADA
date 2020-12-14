#pragma once

#include "grid/partition.hpp"
#include "grid/tiled_data.hpp"
#include "grid/boundary.hpp"
#include "utils/constexpr_functions.hpp"
#include "loops/flatten_index.hpp"

namespace JADA {

template <size_t N, class Container, class Op>
static void apply(const Container&    in,
                  Container&          out,
                  const Partition<N>& p,
                  [[maybe_unused]] Op op) {

    Utils::runtime_assert(in.size() == out.size(), "Array size mismatch.");

    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;
 
    for (auto pos : loop(p)) {

        auto idx = size_t(
            flatten<N, StorageOrder::RowMajor>(p.parent_dimensions(), pos));

        out[idx] = Op::apply(storage_t(&in[idx]));
    }
}




template <size_t N, class Container, class Op>
static void apply(
                  [[maybe_unused]] const Container&    in1,
                  [[maybe_unused]] const Container&    in2,
                  [[maybe_unused]] Container&          out,
                  [[maybe_unused]] const Partition<N>& p1,
                  [[maybe_unused]] const Partition<N>& p2,
                  [[maybe_unused]] const position<N>&  direction,
                  [[maybe_unused]] Op op) {

    Utils::runtime_assert(in1.size() == out.size(), "Array size mismatch.");
    
    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;


    [[maybe_unused]] storage_t used;


    //first index that this routine will compute relative to the boundary, always on owner side and max -0
    [[maybe_unused]] idx_t first_computable_idx = -tile_t::barrier_end() + 1;

    //first index that this routine will read relative to the boundary, 
    //can be on either owner (negative) or neighbour side (positive) 
    [[maybe_unused]] idx_t first_idx_to_read = first_computable_idx + tile_t::get_min();


    //total number of boundary stencils
    [[maybe_unused]] static constexpr size_t n_boundary_stencils = size_t(tile_t::barrier_end());


    //total width of the points effected by the stencil (boundary tile witdth)
    [[maybe_unused]] static constexpr size_t total_boundary_width = n_boundary_stencils + tile_t::get_width() - 1;


    //last index that this routine will read relative to the boundary, 
    //can be on either owner (negative) or neighbour side (positive)
    [[maybe_unused]] idx_t  last_idx_to_read = first_idx_to_read + idx_t(total_boundary_width);

    /*
    std::cout << first_computable_idx << std::endl;
    std::cout << first_idx_to_read << std::endl;
    std::cout << n_boundary_stencils << std::endl;
    std::cout << total_boundary_width << std::endl;
    std::cout << last_idx_to_read << std::endl;
    std::exit(1);
    */
    for (auto [p_owner, p_neigh] : loop(p1, p2, direction)){

        [[maybe_unused]] std::array<ET, total_boundary_width> temp;        


        size_t ii = 0;

        for (idx_t i = first_idx_to_read; i < last_idx_to_read; ++i, ++ii){

            if (i <= 0){
                auto owner_pos = p_owner + (direction * i);
                auto owner_i   = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), owner_pos));
                temp[ii] = in1[owner_i];
            }
            else {
                auto neigh_pos = p_neigh + (direction * (i-1));
                auto neigh_i =  size_t(flatten<N, StorageOrder::RowMajor>(p2.parent_dimensions(), neigh_pos));
                temp[ii] = in2[neigh_i];
                //temp[ii] = 100;

            }

        }


        for (auto t : temp){
            std::cout << t << std::endl;
        }
        


        for (size_t i = 0; i < n_boundary_stencils; ++i){
            std::array<ET, tile_t::get_width()> storage;
            std::copy(std::begin(temp) + i, std::begin(temp) + i + tile_t::get_width(), std::begin(storage));

            storage_t s(storage);
            
            auto out_pos = p_owner - (direction * idx_t(n_boundary_stencils - i) - 1);
            auto out_i   = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), out_pos));
            out[out_i] = Op::apply(s);

        }






    }
    
}






} // namespace JADA
