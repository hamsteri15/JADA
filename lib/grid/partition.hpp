#pragma once

#include <array>

#include "grid/direction.hpp"
#include "grid/grid_base.hpp"
#include "loops/position.hpp"
#include "loops/serial_index_loops.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {


template <idx_t Dim> struct Partition {

    Partition(std::array<idx_t, Dim> parent_dims,
              std::array<idx_t, Dim> begin,
              std::array<idx_t, Dim> extent)
        : m_parent_dims(parent_dims)
        , m_begin(begin)
        , m_extent(extent) {

            Utils::runtime_assert(m_begin >= std::array<idx_t, Dim>{}, "Partition dimensions invalid.");
            Utils::runtime_assert(get_end() <= m_parent_dims, "Partition dimensions invalid.");


        }

    Partition(std::array<idx_t, Dim> parent_dims,
              Direction              dir,
              idx_t                  begin_idx,
              idx_t                  width)
        : Partition(parent_dims,
                    get_begin(dir, begin_idx),
                    get_extent(parent_dims, dir, width)) {}


    index_generator<1> get_loop() const{
        return serial_index(m_begin, get_end(), m_parent_dims);
    }



    std::array<idx_t, Dim> get_begin() const {
        return m_begin;
    }

    std::array<idx_t, Dim> get_end() const {
        std::array<idx_t, Dim> end;

        for (idx_t i = 0; i < Dim; ++i){
            end[i] = m_begin[i] + m_extent[i];
        }
        return end;
    }

    idx_t size() const {

        return std::accumulate(m_extent.begin(), m_extent.end(), idx_t(1), std::multiplies<idx_t>{});

    }


private:
    std::array<idx_t, Dim> m_parent_dims;
    std::array<idx_t, Dim> m_begin;
    std::array<idx_t, Dim> m_extent;

    static std::array<idx_t, Dim> get_begin(Direction dir, idx_t begin_idx) {
        std::array<idx_t, Dim> begin{};
        idx_t                  idx = DirectionMap<Dim>::dir_to_idx(dir);
        begin[idx]                 = begin_idx;
        return begin;
    }


    static std::array<idx_t, Dim>
    get_extent(std::array<idx_t, Dim> parent_dims, Direction dir, idx_t width) {
        std::array<idx_t, Dim> extent = parent_dims;
        idx_t                  idx    = DirectionMap<Dim>::dir_to_idx(dir);
        extent[idx]                   = width;
        return extent;
    }
};

} // namespace JADA