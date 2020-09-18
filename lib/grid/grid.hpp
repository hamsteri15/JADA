#pragma once

#include <numeric>
#include <array>

#include "grid/direction.hpp"
#include "loops/index_type.hpp"

namespace JADA {

template <idx_t Dim> struct Grid {

    static_assert(Dim <= 3, "Only up to grid 3 dimensions supported.");

    Grid() = default;

    ///
    ///@brief Construct a new Grid object
    ///
    ///@param Number of points per Dim in the grid
    ///
    Grid(std::array<idx_t, Dim> dimensions)
        : m_dimensions(dimensions) {}

    ///
    ///@brief Get the total grid point count
    ///
    ///@return idx_t number of points in the grid
    ///
    idx_t size() const {
        return std::accumulate(m_dimensions.begin(),
                               m_dimensions.end(),
                               idx_t(1),
                               std::multiplies<idx_t>());
    }

    ///
    ///@brief Get the number of points in each direction
    ///
    ///@return const std::array<idx_t, Dim>& number of points in all directions
    ///
    const std::array<idx_t, Dim>& dimensions() const { return m_dimensions; }

    ///
    ///@brief Get the direction specific grid point count
    ///
    ///@param dir direction to query the point count 
    ///@return idx_t the point count in the input direction
    ///
    idx_t dimension(Direction dir) const {
        const idx_t dir_idx = DirectionMap<Dim>::dir_to_idx(dir);
        return m_dimensions[dir_idx];
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

private:
    std::array<idx_t, Dim> m_dimensions;
};

} // namespace JADA