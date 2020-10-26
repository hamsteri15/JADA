#pragma once
#include <numeric> //std::accumulate
#include "grid/grid_dimensions.hpp"
#include "grid/block.hpp"
#include "loops/serial_index_loops.hpp"
#include "loops/md_index_loops.hpp"


namespace JADA {

template <class GT> struct GridBase {

    // CRTP inject casts to derived types
    constexpr GT&       operator~() noexcept { 
        return static_cast<GT&>(*this); 
    }

    constexpr const GT& operator~() const noexcept {
        return static_cast<const GT&>(*this);
    }
};

// Global functions

template <typename GT> GT& crtp_cast(GridBase<GT>& grid) { 
    return ~grid; 
}

template <typename GT> const GT& crtp_cast(const GridBase<GT>& grid) {
    return ~grid;
}

template <typename GT> inline idx_t size(const GridBase<GT>& grid) noexcept {
    return (~grid).size();
}

template <typename GT>
inline constexpr idx_t n_spatial_dims(const GridBase<GT>& grid) noexcept {
    return (~grid).n_spatial_dims();
}

template <typename GT>
inline auto dimensions(const GridBase<GT>& grid) noexcept {
    return (~grid).dimensions();
}

template<typename GT>
inline index_generator<1> indices(const GridBase<GT>& grid) noexcept {
    return serial_index(position<1>{0}, position<1>{size(grid)}, position<1>{size(grid)});
}


template<idx_t N>
struct Grid : public GridBase<Grid<N>>{

    static_assert(N <= 3, "Only up to 3 spatial dimensions supported");


    Grid() = default;
    explicit Grid(GridDims<N> dims) : m_dims(dims) {}


    inline constexpr idx_t n_spatial_dims() const {
        return N;
    }

    inline idx_t size() const {
        return std::accumulate(m_dims.begin(), m_dims.end(), idx_t(1), std::multiplies<idx_t>{});
    }

    inline GridDims<N> dimensions() const {
        return m_dims;
    }

private:
    GridDims<N> m_dims;

};


template<idx_t N>
inline index_generator<N> md_indices(const Grid<N>& grid){
    return md_indices(GridDims<N>{}, dimensions(grid));
}




template<idx_t N>
struct StructuredGrid : public Grid<N>{


    StructuredGrid(Block<N> block) : 
    Grid<N>(block.dimensions), m_block(block) {}

    /*

    StructuredGrid(GridDims<N> dims) : 
    Grid<N>(dims),
    m_domain(domain)
    {}

    */


private:
    Block<N> m_block;


};







} // namespace JADA