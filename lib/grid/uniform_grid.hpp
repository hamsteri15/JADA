#pragma once
#include "grid/grid.hpp"

namespace JADA{

template<idx_t Dim>
struct UniformGrid : public Grid<Dim>{

    UniformGrid(std::array<idx_t, Dim> dimensions, std::array<double, Dim> lengths) :
    Grid<Dim>(dimensions),
    m_stepsize(compute_stepsize(dimensions, lengths))
    {}


    std::array<double, Dim> stepsize() const {return m_stepsize;}

private:
    std::array<double, Dim> m_stepsize;


    std::array<double, Dim> compute_stepsize(std::array<idx_t, Dim> dimensions, std::array<double, Dim> lengths){

        std::array<double, Dim> stepsize;
        for (idx_t i = 0; i < Dim; ++i){
            stepsize[i] = lengths[i] / double(dimensions[i]);
        }
        return stepsize;
    }

};

}