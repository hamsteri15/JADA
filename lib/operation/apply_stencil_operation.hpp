#pragma once

#include "grid/grid.hpp"
#include "stencil/stencil_picker.hpp"

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
void apply_stencil_operation(const Iterable& in, Iterable& out, IndexGenerator loop, index_type shift, Method method){

    using namespace Math;

    for (auto [idx] : loop){

        auto   tuple = StencilPicker::pick_stencil(in, method.get_idx(), idx, shift); //pick a stencil
        auto   func  = [&](auto&... wr) { return Method::apply(wr...); }; //apply an operation for it

        out[idx] = std::apply(func, tuple); 
    }


}



}