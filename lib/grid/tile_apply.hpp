#pragma once

#include "grid/partition.hpp"
#include "grid/tiled_data.hpp"
#include "loops/flatten_index.hpp"

namespace JADA {

template <size_t N, class Container, class Op>
static void apply(const Container&    in,
                  Container&          out,
                  const Partition<N>& p,
                  [[maybe_unused]] Op op) {

    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;

    for (auto pos : loop(p)) {

        auto idx = size_t(
            flatten<N, StorageOrder::RowMajor>(p.parent_dimensions(), pos));

        out[idx] = Op::apply(storage_t(&in[idx]));
    }
}
/*
template <size_t N, class Container, class Op>
static void apply(const Container&    owner_d,
                  const Container&    neigh_d,
                  Container&          out,
                  const Partition<N>& owner_p,
                  const Partition<N>& neigh_p,
                  [[maybe_unused]] Op op) {

    using ET        = Container::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;

    for (auto pos : loop(owner_p)) {

        

        

        auto idx = size_t(
            flatten<N, StorageOrder::RowMajor>(p.parent_dimensions(), pos));

        out[idx] = Op::apply(storage_t(&in[idx]));
        
    }
}
*/

} // namespace JADA
