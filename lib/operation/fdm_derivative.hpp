#pragma once

#include "grid/direction.hpp"
#include "grid/grid.hpp"
#include "grid/grid_loops.hpp"
#include "loops/index_type.hpp"
#include "loops/index_generator.hpp"
#include "stencil/stencil_picker.hpp"

namespace JADA{

template<idx_t Dim, Direction dir, class StencilOp>
struct FdmDerivative{

    FdmDerivative(const Grid<Dim>& grid) : 
    m_grid(grid), 
    m_interior_loop(directional_loop(grid, dir, StencilOp{})) {}



    template<class Iterable>
    void operator()(const Iterable& in, Iterable& out) {

        //TODO: consider if necessessary
        Utils::runtime_assert(std::size(in) == std::size(m_grid), "The grid size differes from input array size.");
        Utils::runtime_assert(std::size(out) == std::size(m_grid), "The grid size differes from output array size.");



        for (auto [idx] : m_interior_loop){

            //pick a stencil
            auto   tuple = StencilPicker::pick_stencil(in, StencilOp::get_indices(), idx, m_grid.offset(dir));
            //apply operation
            auto   func  = [&](auto&... wr) { return StencilOp::apply(wr...); };

            out[idx] = std::apply(func, tuple); 
        }



    }

private:
    const Grid<Dim>&    m_grid;
    index_generator<1>  m_interior_loop;

};





}