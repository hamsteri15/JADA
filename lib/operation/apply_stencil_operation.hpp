#pragma once

#include "grid/grid.hpp"
#include "grid/direction.hpp"
#include "stencil/stencil_picker.hpp"
#include "grid/grid_loops.hpp"
#include "utils/runtime_assert.hpp"

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
///@param offset the offset between elements. For example (ni * nj) for a 3D grid when indexing the 'k' direction
///@param method the operation to apply
///
template<class Iterable, class IndexGenerator, class Method>
void apply_stencil_operation(const Iterable& in, Iterable& out, IndexGenerator loop, idx_t offset, Method method){

    //TODO: consider if this is necessary, this requires the Iterables to follow the standard containers
    Utils::runtime_assert(in.size() == out.size(), "The input array size different than output array size");

    
    for (auto [idx] : loop){

        auto   tuple = StencilPicker::pick_stencil(in, method.get_idx(), idx, offset); //pick a stencil
        auto   func  = [&](auto&... wr) { return Method::apply(wr...); }; //apply an operation for it

        out[idx] = std::apply(func, tuple); 
    }


}



template <class Iterable, class Method, idx_t N>
void apply_stencil_operation( const Iterable& in,
                              Iterable&       out,
                              const Grid<N>&  grid,
                              Direction       dir,
                              Method                           method) {

    //TODO: consider if necessessary
    Utils::runtime_assert(std::size(in) == std::size(grid), "The grid size differes from input array size.");
    Utils::runtime_assert(std::size(out) == std::size(grid), "The grid size differes from output array size.");

    apply_stencil_operation(in, out, directional_loop(grid,dir,method), grid.offset(dir));

}
}