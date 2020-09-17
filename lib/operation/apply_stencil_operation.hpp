#pragma once

#include "grid/grid.hpp"
#include "grid/direction.hpp"
#include "stencil/stencil_picker.hpp"
#include "loops/index_generator.hpp"
#include "loops/serial_index_loops.hpp"

namespace JADA{

///
///@brief Applies a stencil operation for the input array for indices defined by the loop parameter
///
///@tparam Iterable any indexable type 
///@tparam IndexGenerator generator for the loop
///@tparam Method the method to apply
///@param in the input container
///@param out the output container
///@param loop the index generator
///@param shift the shift between elements. For example (ni * nj) for a 3D grid when indexing the 'k' direction
///@param method the operation to apply
///
template<class Iterable, class IndexGenerator, class Method>
void apply_stencil_operation(const Iterable& in, Iterable& out, IndexGenerator loop, idx_t shift, Method method){


    for (auto [idx] : loop){

        auto   tuple = StencilPicker::pick_stencil(in, method.get_idx(), idx, shift); //pick a stencil
        auto   func  = [&](auto&... wr) { return Method::apply(wr...); }; //apply an operation for it

        out[idx] = std::apply(func, tuple); 
    }


}



template <class Iterable, class Method, idx_t N>
void apply_stencil_operation([[maybe_unused]] const Iterable& in,
                             [[maybe_unused]] Iterable&       out,
                             [[maybe_unused]] const Grid<N>&  grid,
                             [[maybe_unused]] Direction       dir,
                             [[maybe_unused]] Method                           method) {

    /*
    constexpr idx_t left_gc = method.left_halfwidth();
    constexpr idx_t right_gc = method.right_halfwidth();

    auto loop = grid_stencil_loop(grid, dir, method);
    */
    /*
    switch (dir){

        case Direction::i:



        default: break;

    }
    */

    

    /*
    for (auto [idx] : loop){

        auto   tuple = StencilPicker::pick_stencil(in, method.get_idx(), idx, shift); //pick a stencil
        auto   func  = [&](auto&... wr) { return Method::apply(wr...); }; //apply an operation for it

        out[idx] = std::apply(func, tuple); 
    }

    */
}
}