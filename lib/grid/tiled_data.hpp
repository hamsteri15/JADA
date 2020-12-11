#pragma once

#include <array>
#include "loops/index_type.hpp"
#include "grid/tile.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{

template<class tile_t, class ET>
struct TiledData{


    static constexpr size_t N  = tile_t::get_width();
    static constexpr idx_t  min = tile_t::get_min();
    static constexpr idx_t  max = tile_t::get_max();


    TiledData() = default;

    TiledData(const ET* ptr) {

        for (idx_t i = min; i != max + 1; ++i){

            idx_t offset = 1;
            m_data[idx_convert(i)] = ptr[i * offset];
        } 

    }

    TiledData(const ET* ptr_owner, const ET* ptr_neigh, idx_t NN){



        for (idx_t i = min; i != min + idx_t(NN); ++i){
            idx_t offset1 = 1;
            m_data[idx_convert(i)] = ptr_owner[i * offset1];
        }

        for (idx_t i = min + idx_t(NN); i != max + 1; ++i){
            idx_t offset2 = 1;
            m_data[idx_convert(i)] = ptr_neigh[i * offset2];

        }


    }



    ET operator()(idx_t i) const{
        return m_data[idx_convert(i)];
    }

private:
    std::array<ET, N> m_data;


    static constexpr size_t idx_convert(idx_t i) {
        Utils::runtime_assert(i >= min, "Tile index out of bounds");
        Utils::runtime_assert(i <= max, "Tile index out of bounds");
        return size_t(i + std::abs(min));
    }


};






}