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

    /*
    Utils::runtime_assert(in1.size() == out.size(), "Array size mismatch.");
    
    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;

    [[maybe_unused]] storage_t t;

    for (auto [p_owner, p_neigh] : loop(p1, p2, direction)){

        //[-2, -1, 0]
        for (idx_t i = tile_t::get_min(); i != 0; ++i){


            auto p_data  = p_owner + direction * (i+1); 
            auto i_data  = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), p_data));
            auto i_owner = size_t(flatten<N, StorageOrder::RowMajor>(p1.parent_dimensions(), p_owner));
            auto i_neigh = size_t(flatten<N, StorageOrder::RowMajor>(p2.parent_dimensions(), p_neigh));


            storage_t s(&in1[i_owner], &in2[i_neigh], size_t(i));
            out[i_data] = Op::apply(s);

            //out[i_data] = 1;
        }

    }
    */
}






} // namespace JADA
