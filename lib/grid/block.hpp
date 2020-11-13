#pragma once

#include "grid/point.hpp"
#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <size_t N> using BlockDensity    = std::array<idx_t, N>;
template <size_t N> using BlockDimensions = std::array<double, N>;

static constexpr int BLOCK_ID_NULL = -42;

template <size_t Dim> class Block {

    Block() = default;

    explicit Block(Point<Dim>        p0,
                   Point<Dim>        p1,
                   int               id,
                   BlockDensity<Dim> density)
        : m_p0(p0)
        , m_p1(p1)
        , m_id(id)
        , m_density(density) {

        Utils::runtime_assert(std::none_of(std::begin(m_density),
                                           std::end(m_density),
                                           [](auto idx) { return idx == 0; }),
                              "Zero density block");

        Utils::runtime_assert(std::equal(std::begin(m_p0),
                                         std::end(m_p0),
                                         std::begin(m_p1),
                                         std::end(m_p1),
                                         std::less_equal{}),
                              "Block point p0 can not be larger than p1");
    }

private:
    Point<Dim>        m_p0;
    Point<Dim>        m_p1;
    int               m_id;
    BlockDensity<Dim> m_density;
};

/*

template<size_t Dim> struct SubBlock : public Block<Dim>{

    SubBlock() = default;

    explicit SubBlock(Block<Dim> parent, )



};
*/




} // namespace JADA