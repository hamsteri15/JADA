#pragma once
#include "grid/grid.hpp"

namespace JADA{

template<idx_t Dim>
struct UniformGrid : public Grid<Dim>{

    UniformGrid(std::array<idx_t, Dim> dimensions, std::array<idx_t, Dim> barriers, std::array<double, Dim> stepsize) :
    Grid<Dim>(dimensions, barriers),
    m_stepsize(stepsize)
    {}


    std::array<double, Dim> stepsize() const {return m_stepsize;}

private:
    std::array<double, Dim> m_stepsize;

};

}