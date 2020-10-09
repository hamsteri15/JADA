#pragma once

#include <numeric>
#include <array>

#include "grid/point.hpp"
#include "grid/grid_dimensions.hpp"
#include "grid/direction.hpp"
#include "loops/index_type.hpp"
#include "loops/serial_index_loops.hpp"

namespace JADA {

template<idx_t N, class Derived>
struct Grid{


    static constexpr idx_t Dim = N;

    ///
    ///@brief CRTP injection of dimensions function
    ///
    ///@return GridDims<Dim> number of nodes in each spatial direction
    ///
    GridDims<Dim> dimensions() const {
        return static_cast<const Derived*>(this)->get_dimensions();
    }


    ///
    ///@brief CRTP injection of points function
    ///
    ///@return const std::vector<std::array<idx_t, Dim>>& all point coordinates of the grid
    ///
    const std::vector<Point<Dim>>& points() const {
        return static_cast<const Derived*>(this)->get_dimensions();
    }


    ///
    ///@brief Total point count in the grid
    ///
    ///@return idx_t total point count 
    ///
    idx_t size() const {
        return elementwise_product(dimensions());
    }


    ///
    ///@brief Get a loop over all indices of the grid
    ///
    ///@return index_generator<1> an index generator returning the indices
    ///
    index_generator<1> get_loop() const{
        return serial_index(std::array<idx_t, Dim>{}, dimensions(), dimensions());
    }



    ///
    ///@brief Get the direction specific grid point count
    ///
    ///@param dir direction to query the point count 
    ///@return idx_t the point count in the input direction
    ///
    idx_t dimension(Direction dir) const {
        const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        return dimensions()[dir_idx];
    }

    ///
    ///@brief Get the offset between successive elements in the input direction.
    ///For example, for a 3D grid, the offsets in directions {i,j,k} are {1, ni,
    ///nj*ni}
    ///
    ///@param dir the direction to query the offset in
    ///@return idx_t the offset in the direction dir
    ///
    idx_t offset(Direction dir) const {
        switch (dir) {
        case Direction::i: 
            return 1;
        case Direction::j: 
            return dimension(Direction::i);
        case Direction::k:
            return dimension(Direction::i) * dimension(Direction::j);
        default: throw std::runtime_error("Invalid direction");
        }
    }



protected:


    ///
    ///@brief Computes an elementwise product of the given array
    ///
    ///@tparam ElementType any type that supports multiplication
    ///@param array an array to multiply
    ///@return ElementType the product of the elements  
    ///
    template<class ElementType>
    static ElementType elementwise_product(std::array<ElementType, Dim> array){

        return std::accumulate(array.begin(),
                               array.end(),
                               ElementType(1),
                               std::multiplies<ElementType>());

    }



};


} // namespace JADA