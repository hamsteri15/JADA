#pragma once

#include "grid/partition.hpp"
#include "grid/tiled_data.hpp"
#include "grid/boundary.hpp"
#include "utils/constexpr_functions.hpp"
#include "loops/flatten_index.hpp"

namespace JADA {

template <size_t N, class Container, class Op>
static void apply(const Container&    in,
                  Container&          out,
                  const Partition<N>& p,
                  [[maybe_unused]] Op op) {

    Utils::runtime_assert(in.size() == out.size(), "Array size mismatch.");

    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;

    for (auto pos : loop(p)) {

        auto idx = size_t(
            flatten<N, StorageOrder::RowMajor>(p.parent_dimensions(), pos));

        out[idx] = Op::apply(storage_t(&in[idx]));
    }
}


template <size_t N, class Container, class Op>
static void apply(
                  [[maybe_unused]] const Container&    in1,
                  [[maybe_unused]] const Container&    in2,
                  [[maybe_unused]] Container&          out,
                  [[maybe_unused]] const Partition<N>& p1,
                  [[maybe_unused]] const Partition<N>& p2,
                  [[maybe_unused]] const position<N>&  direction,
                  [[maybe_unused]] Op op) {

    Utils::runtime_assert(in1.size() == out.size(), "Array size mismatch.");
    
    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;


    [[maybe_unused]] storage_t used;

    for (auto [p_owner, p_neigh] : loop(p1, p2, direction)){

        auto p = p_owner - direction * tile_t::get_max() + 1;
        auto o_end   = p_owner + direction;
        auto n_start = p_neigh;


        //auto pp = tilet_t::get_max() + 1;
        //auto ll = p + tile_t::get_min();


        //idx_t n_read_owner = 


        while (p != o_end){

            auto l = p       + direction * tile_t::get_min();
            auto o_start = l;

            size_t idx_temp = 0;
            std::array<ET, tile_t::get_width()> stencil;
            for (auto b_pos : md_indices(o_start, o_end)){
                
                auto b_idx = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), b_pos));
                stencil[idx_temp] = in1[b_idx];
                idx_temp+=1;
            }

            auto n_end   = n_start + direction * (idx_t(tile_t::get_width() - idx_temp));

            for (auto b_pos : md_indices(n_start, n_end)){
                auto b_idx = size_t(flatten<N, StorageOrder::RowMajor>(p2.parent_dimensions(), b_pos));
                stencil[idx_temp] = in2[b_idx];
                idx_temp += 1;
            }        


            storage_t s(stencil);

            auto out_idx = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), p));

            out[out_idx] = Op::apply(s);

            p += direction;

        }


        

        

    }
}






} // namespace JADA
