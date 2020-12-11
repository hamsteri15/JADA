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
                  [[maybe_unused]] const Boundary<N>&  b,
                  [[maybe_unused]] Op op) {

    /*
    Utils::runtime_assert(in1.size() == out.size(), "Array size mismatch.");
    
    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;

    static constexpr size_t N_tiles = Utils::constexpr_abs(tile_t::get_min()) + 1;





    for (auto pos : loop(b)) {

        for (idx_t i = idx_t(N_tiles); i != 0; --i){

            ///std::cout << i << std::endl;
            auto pos_owner = pos - (b.get_direction() * i);
            //auto pos_neigh = 
            auto idx_owner = size_t(
            flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), pos_owner));





            out[idx_owner] = in1[idx_owner];
        }


        [[maybe_unused]] storage_t temp;


         //Op::apply(storage_t(&in[idx]));
    }
    */
}






} // namespace JADA
