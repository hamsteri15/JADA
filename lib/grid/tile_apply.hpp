#pragma once

#include "grid/boundary.hpp"
#include "grid/md_view.hpp"
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

template <class T, class P> struct DataHandle {

    DataHandle(const T& data, const P& center, const P& dir)
        : m_data(data)
        , m_center(center)
        , m_dir(dir) {}

    const auto operator()(idx_t i) const {
        return m_data(m_center + m_dir * i);
    }

    const T& m_data;
    const P& m_center;
    const P& m_dir;
};

template<size_t N, class Storage, class Op>
static void apply_interior(const MdView<N, Storage>& in, MdView<N, Storage>& out, [[maybe_unused]] Op op){



    Utils::runtime_assert(in.size() == out.size(), "Size mismatch in apply interior");

    constexpr auto ori = op.shape.get_orientation();
    constexpr auto dir = to_direction<N>(ori);

    auto width_begin = op.shape.barrier_begin(ori);
    auto width_end = op.shape.barrier_end(ori);


    auto interior = get_interior_subblock(in.get_block(), dir, width_begin, width_end);


    for (auto pos : loop(interior)){
        out(pos) = op.apply(DataHandle(in, pos, dir));
    }


}

/*

apply_boarder(MdView owner, MdView out, Op op) {


    
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
