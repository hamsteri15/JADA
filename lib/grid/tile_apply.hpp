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

/*
template<size_t N, class Tile>
position<N> get_boundary_offset_end(position<N> direction, Tile t){

    return direction * t::

}
*/



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


    [[maybe_unused]] idx_t offset_p1 = 1;
    [[maybe_unused]] idx_t offset_p2 = 1;



    for (auto [p_owner, p_neigh] : loop(p1, p2, direction)){

        //compute maximum owner iterator
        const auto end_i =  size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), p_owner));
        const auto* owner_max = &in1[end_i] + 1;

        //compute the minimum neighbour iterator
        const auto begin_i = size_t(flatten<N, StorageOrder::RowMajor>(p2.parent_dimensions(), p_neigh));
        const auto* neigh_min = &in2[begin_i];


        //compute the current output iterator
        const auto current_pos = p_owner - direction * tile_t::get_max() + 1;
        const auto current_i = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), current_pos));
       
        auto* out_begin = &out[current_i];
        const auto* out_end = &out[end_i] + 1;


        const auto stencil_begin_pos = current_pos + direction * tile_t::get_min();
        const auto stencil_begin_i   = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), stencil_begin_pos));
        auto* stencil_min            = &in1[stencil_begin_i];


        for (auto* current = out_begin; current != out_end; current+=offset_p1){
        

            storage_t s(stencil_min, owner_max, neigh_min );
            *current = Op::apply(s);

            stencil_min += offset_p1;


        }

        

        

    }
}






} // namespace JADA
