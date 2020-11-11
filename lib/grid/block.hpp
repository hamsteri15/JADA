#pragma once

#include "grid/point.hpp"
#include "loops/index_type.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA {

template <size_t N> using BlockDensity    = std::array<idx_t, N>;
template <size_t N> using BlockDimensions = std::array<double, N>;


enum class BlockFaceId{
    Interface, Physical
};

template<size_t Dim> struct BlockFaceIdList{

    using id_pair_t = std::pair<BlockFaceId, BlockFaceId>;


    BlockFaceIdList() {
        for (auto& id : m_ids){
            id.first = BlockFaceId::Physical;
            id.second = BlockFaceId::Physical;
        }
    }

    explicit BlockFaceIdList(std::array<bool, Dim> periodicity){

        for (size_t i = 0; i < Dim; ++i){
            if (periodicity[i]) {
                m_ids[i].first = BlockFaceId::Interface;
                m_ids[i].second = BlockFaceId::Interface;
            }
            else {
                m_ids[i].first = BlockFaceId::Physical;
                m_ids[i].second = BlockFaceId::Physical;
            }
        }     

    }

    explicit BlockFaceIdList(std::array<id_pair_t, Dim> ids) : 
    m_ids(ids)
    {}

    



private:
    std::array<id_pair_t, Dim> m_ids;

};


template <size_t Dim> struct Block {

    Block() = default;

    explicit Block(BlockDensity<Dim> density_,
                   Point<Dim>        p0_,
                   Point<Dim>        p1_,
                   int               id_)
        : density(density_)
        , p0(p0_)
        , p1(p1_)
        , id(id_) {

        Utils::runtime_assert(std::none_of(std::begin(density_),
                                           std::end(density_),
                                           [](auto idx) { return idx == 0; }),
                              "Zero density block");

        Utils::runtime_assert(std::equal(std::begin(p0),
                                         std::end(p0),
                                         std::begin(p1),
                                         std::end(p1),
                                         std::less_equal{}),
                              "Block point p0 can not be larger than p1");
    }

    BlockDensity<Dim> density;
    Point<Dim>        p0;
    Point<Dim>        p1;
    int               id;
};

} // namespace JADA