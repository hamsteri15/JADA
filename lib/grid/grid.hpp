#pragma once
#include <array>

#include "loops/index_type.hpp"

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


}