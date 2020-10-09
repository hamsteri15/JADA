#pragma once
#include "grid/grid.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{


template<idx_t N>
struct UniformGrid : public Grid<N, UniformGrid<N>>{



    UniformGrid() = default;

    ///
    ///@brief Construct from a given point count. Sets unity stepsize in each direction.
    ///
    ///@param dimensions Number of points in each direction.
    ///
    UniformGrid(GridDims<N> dimensions) : 
    m_dimensions(dimensions)
    {
        for (idx_t i = 0; i < N; ++i){
            m_stepsize[i] = double(1);
        }

    }


    //UniformGrid(Point<N> begin, Point<N> end, GridDims<N> dimensions)








    std::array<double, N> stepsize() const {
        return m_stepsize;
    }

    GridDims<N> get_dimensions() const {
        return m_dimensions;
    }




private:
    GridDims<N> m_dimensions;
    std::array<double, N> m_stepsize;
    std::vector<Point<N>> m_points;



    static std::array<double, N> compute_stepsize(GridDims<N> dimensions, std::array<double, N> lengths){

        std::array<double, N> stepsize;
        for (idx_t i = 0; i < N; ++i){
            Utils::runtime_assert(lengths[i] >= 0, "Negative grid length");
            stepsize[i] = lengths[i] / double(dimensions[i]);
        }
        return stepsize;
    }



    static std::array<double, N> point_difference(Point<N> begin, Point<N> end) {

        Point<N> diff;
        for (idx_t i = 0; i < N; ++i){
            diff[i] = end[i] - begin[i];
        }
        return diff;

    }

    static std::vector<Point<N>> create_points(Point<N> begin, Point<N> end, GridDims<N> dimensions){

        auto L = point_difference(begin, end);
        auto stepsize = compute_stepsize(dimensions, L);

        idx_t n_points = elementwise_product(dimensions);
        std::vector<Point<N>> points(n_points);

        throw std::runtime_error("Not implemented.");

        /*
for (size_t i = coords.i_begin(); i < coords.i_end(); i++){
	for (size_t j = coords.j_begin(); j < coords.j_end(); j++){
	for (size_t k = coords.k_begin(); k < coords.k_end(); k++){
		
		//boundary points at 1/2*delta from the boundary
		x = x0 + dx*0.5 + (i-ngc)*dx;
		y = y0 + dy*0.5 + (j-ngc)*dy;
		z = z0 + dz*0.5 + (k-ngc)*dz;
        */


        return points;



    }




};





}