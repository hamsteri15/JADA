#pragma once
#include <array>

#include "loops/md_index_loops.hpp"
#include "loops/serial_index_loops.hpp"


namespace JADA{



template<idx_t Dim>
struct Grid{

    static_assert(Dim <= 3, "Only up to grid 3 dimensions supported.");


    Grid() = default;

    ///
    ///@brief Construct a new Grid object
    ///
    ///@param Number of points per Dim in the grid
    ///
    Grid(std::array<idx_t, Dim> dimensions) :
    m_dimensions(dimensions)
    {}


    const std::array<idx_t, Dim>& dimensions() const{
        return m_dimensions;
    }

private:

    std::array<idx_t, Dim> m_dimensions;

};






/*
template <idx_t Dim>
inline static index_generator<1> serial_loop_interior(const Grid<Dim>& grid) {

    return serial_index(grid.begin(), grid.end(), grid.padded_dimensions());
}

template <idx_t Dim>
inline static index_generator<Dim> md_loop_interior(const Grid<Dim>& grid) {

    return md_indices(grid.begin(), grid.end());
}
*/




}