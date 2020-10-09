#pragma once
#include "grid/grid.hpp"

namespace JADA{


template<idx_t N>
struct UniformGrid : public Grid<N, UniformGrid<N>>{



    UniformGrid() = default;

    UniformGrid(std::array<idx_t, N> dimensions) : 
    m_dimensions(dimensions)
    {
        for (idx_t i = 0; i < N; ++i){
            m_stepsize[i] = double(1);
        }
    }



    UniformGrid(std::array<idx_t, N> dimensions, std::array<double, N> lengths) :
    m_stepsize(compute_stepsize(dimensions, lengths))
    {}




    std::array<double, N> stepsize() const {
        return m_stepsize;
    }

    std::array<idx_t, N> get_dimensions() const {
        return m_dimensions;
    }




private:
    std::array<idx_t, N> m_dimensions;
    std::array<double, N> m_stepsize;


    std::array<double, N> compute_stepsize(std::array<idx_t, N> dimensions, std::array<double, N> lengths){

        std::array<double, N> stepsize;
        for (idx_t i = 0; i < N; ++i){
            stepsize[i] = lengths[i] / double(dimensions[i]);
        }
        return stepsize;
    }

};





}