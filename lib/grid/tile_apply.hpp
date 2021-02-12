#pragma once

#include "grid/boundary.hpp"
#include "grid/data.hpp"
#include "grid/tile.hpp"
#include "utils/constexpr_functions.hpp"
#include "loops/flatten_index.hpp"

namespace JADA {


struct TEMP_OP1 {

    static constexpr Tile shape = Tile(-2, 2, Orientation(0));

    
    template<class IT>
    static auto apply(const IT f){
        return f(-2) + f(-1) + f(0) + f(1) + f(2);
    } 
};


template<class ConstIt>
struct DataHandle{

    constexpr DataHandle(ConstIt data, idx_t offset) : m_data(data), m_offset(offset){}


    const auto operator() (idx_t i) const {
        return m_data[i*m_offset];
    }


    ConstIt m_data;    
    idx_t m_offset;
};





template<size_t N, class Storage, class Op>
static void apply_interior(const Data<N, Storage>& in, Data<N, Storage>& out, [[maybe_unused]] Op op){




    Utils::runtime_assert(in.size() == out.size(), "Size mismatch in apply interior");

    constexpr auto ori = op.shape.get_orientation();
    constexpr auto dir = to_direction<N>(ori);

    auto width_begin = op.shape.barrier_begin(ori);
    auto width_end = op.shape.barrier_end(ori);


    auto interior = get_interior_subblock(in.get_block(), dir, width_begin, width_end);

    auto block_dims = in.get_block().dimensions();

    const auto offsets = get_shifts<N, StorageOrder::RowMajor>(block_dims);

    [[maybe_unused]] const auto offset = idx_t(offsets[ori.id()]);

    const auto& in_data = in.get_storage().data();
    auto* out_data = out.get_storage().data();


    for (auto pos : loop(interior)){

        auto idx = flatten<N, StorageOrder::RowMajor>(block_dims, pos );

        out_data[idx] = op.apply(DataHandle(&in_data[idx], offset));


    }


}

/*

apply_boarder(Data owner, Data out, Op op) {


    
    1) Compute owner_width
    2) Compute neighbour width
    3) Compute owner start
    4) Compute neighbour start
    5) Slice owner data
    6) Get neighbour data
    7) Combine datas
    8) Call apply_interior




}
*/






















} // namespace JADA
