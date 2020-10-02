#pragma once

#include "operation/stencil_operation.hpp"
#include "stencil/stencil_indices.hpp"
#include "grid/direction.hpp"
#include "grid/grid.hpp"
#include "grid/grid_loops.hpp"
#include "loops/index_type.hpp"
#include "loops/index_generator.hpp"
#include "stencil/stencil_picker.hpp"
#include "operation/boundary_location.hpp"


namespace JADA{


template<class Scheme>
struct FdmBoundaryOperation{

    static constexpr auto scheme_indices = Scheme::indices;
    static constexpr idx_t scheme_left_width = Scheme::left_halfwidth(); 
    static constexpr idx_t scheme_right_width = Scheme::right_halfwidth(); 

    template<idx_t Dim>
    static idx_t get_buffer_size(const Grid<Dim>& grid, Direction dir, BoundaryLocation loc){

        auto dims = get_buffer_dims(grid, dir, loc);
        return std::accumulate(dims.begin(), dims.end(), idx_t(1), std::multiplies<idx_t>{});

    }

    template<idx_t Dim>
    static std::array<idx_t, Dim> get_buffer_dims(const Grid<Dim>& grid, Direction dir, BoundaryLocation loc){

        auto dims = grid.dimensions();
        const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        if (loc == BoundaryLocation::begin){
            dims[dir_idx] = scheme_left_width;
        }
        else {
            dims[dir_idx] = scheme_right_width;
        }
        return dims;

    }


    template<idx_t Dim>
    static std::array<idx_t, Dim> get_buffer_begin(const Grid<Dim>& grid, Direction dir, BoundaryLocation loc){

        std::array<idx_t, Dim> begin{};

        if (loc == BoundaryLocation::begin){
            return begin;
        }

        const auto dims = grid.dimensions();
        const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        begin[dir_idx] -= dims[dir_idx] - scheme_right_width;

        return begin;

    }


    template<idx_t Dim>
    static std::array<idx_t, Dim> get_buffer_end(const Grid<Dim>& grid, Direction dir, BoundaryLocation loc){

        auto begin = get_buffer_begin(grid, dir, loc);
        auto dims  = get_buffer_dims(grid, dir, loc);
        std::array<idx_t, Dim> end;

        for (idx_t i = 0; i < Dim; ++i){
            end[i] = begin[i] + dims[i];
        }
        return end;

    }

    


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

        
        


        /*
        for (idx_t i = 0; i < this->scheme_left_width; ++i){

            auto new_op = mirror_front(this->scheme_indices, this->scheme_left_width-1);


        }
        */

    }


private:
    const Grid<Dim>& m_grid; 

};




}