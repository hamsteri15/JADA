#pragma once

#include "grid/boundary.hpp"
#include "grid/data.hpp"
#include "utils/constexpr_functions.hpp"
#include "loops/flatten_index.hpp"

namespace JADA {

/*
template<size_t N, class Storage, class Op>
static void apply(const Data<N, Storage>& in, Data<N, Storage>& out, [[maybe_unused]] Op op){

    
    using ET        = Storage::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;

    Utils::runtime_assert(in.size() == out.size(), "Data size mismatch");

    const auto& data_in = in.get_storage();
    auto& data_out = out.get_storage();



    for (auto pos : loop(in.get_block())){
        auto idx = size_t(flatten<N, StorageOrder::RowMajor>(in.get_block().dimensions(), pos));
        data_out[idx] = Op::apply(storage_t(&data_in[idx]));
    }    


}
*/





} // namespace JADA
