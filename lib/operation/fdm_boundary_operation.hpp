#pragma once

#include "operation/stencil_operation.hpp"
#include "stencil/stencil_indices.hpp"
#include "grid/direction.hpp"
#include "grid/grid.hpp"
#include "grid/grid_loops.hpp"
#include "loops/index_type.hpp"
#include "loops/index_generator.hpp"
#include "stencil/stencil_picker.hpp"

namespace JADA{

template<class Scheme>
struct FdmBoundaryOperation{

    static constexpr auto scheme_indices = Scheme::indices;
    static constexpr idx_t scheme_left_width = Scheme::left_halfwidth(); 
    static constexpr idx_t scheme_right_width = Scheme::right_halfwidth(); 


};

template<class Scheme, idx_t Dim, Direction dir>
struct FdmMirrorOperation : public FdmBoundaryOperation<Scheme>{

    /*
    using base_type = FdmBoundaryOperation<Scheme>;
    using base_type::scheme_indices;
    using base_type::scheme_left_width;
    using base_type::scheme_right_width;
    */

    FdmMirrorOperation(const Grid<Dim>& grid) : 
    m_grid(grid)
    {}

    template<class Indexable>
    void do_stuff(const Indexable& in, Indexable& out) const{

        for (idx_t i = 0; i < this->scheme_left_width; ++i){

            auto new_op = mirror_front(this->scheme_indices, this->scheme_left_width-1);


        }


    }


private:
    const Grid<Dim>& m_grid; 

};




}