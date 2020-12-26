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

template<class It, class OutIt>
void pick_boundary(It left, It right, OutIt out, idx_t first, idx_t width, idx_t offset_left, idx_t offset_right){

    
    idx_t last = first + width;

    //read from first to zero from the lhs
    for (idx_t i = first; i < 0; ++i, ++out){
        auto it = left + i * offset_left;
        *out       = *it;
    }

    //read from 1/first to last from the rhs
    for (idx_t i = std::max(first, idx_t(0)); i < last; ++i, ++out){
        auto it = right + (offset_right * i);
        *out       = *it;
    }


}

template<class InIt, class OutIt, class Op>
void rename_me(InIt stencil, OutIt out, idx_t offset, size_t n_boundary_stencils, [[maybe_unused]] Op op){


    using ET        = std::iterator_traits<InIt>::value_type;
    using tile_t    = Op::Shape;
    using storage_t = TiledData<tile_t, ET>;


    for (size_t i = 0; i < n_boundary_stencils; ++i, out+=offset){

        //auto s = storage_t(stencil + i, stencil + i + Op::Shape::get_width());
        //*out = Op::apply(s);
        *out = Op::apply(storage_t(stencil + i, stencil + i + Op::Shape::get_width()));
    }


}

template <size_t N, class Container, class Op>
static void apply_begin(
                  [[maybe_unused]] const Container&    lhs,
                  [[maybe_unused]] const Container&    rhs,
                  [[maybe_unused]] const Partition<N>& p_lhs,
                  [[maybe_unused]] const Partition<N>& p_rhs,
                  [[maybe_unused]] Container&          out,
                  [[maybe_unused]] const position<N>&  direction,
                  [[maybe_unused]] Op op) {

    Utils::runtime_assert(lhs.size() == out.size(), "Array size mismatch.");
    
    using ET        = Container::value_type;
    using tile_t    = Op::Shape;


    //first index that this routine will read relative to the boundary, 
    //can be on either owner (negative) or neighbour side (positive) 
    static constexpr idx_t first_idx_to_read = tile_t::barrier_begin() + 1 + tile_t::get_min();

    //total number of boundary stencils
    static constexpr size_t n_boundary_stencils = size_t(tile_t::barrier_begin());

    //total width of the points effected by the stencil (boundary tile witdth)
    static constexpr size_t total_boundary_width = n_boundary_stencils + tile_t::get_width() - 1;


    std::cout << "First read idx: " << first_idx_to_read << std::endl;
    std::cout << "n_boundary_stencils: " << n_boundary_stencils << std::endl;
    std::cout << "total_boundary_width: " << total_boundary_width << std::endl;




    const idx_t offset_left = 1;
    const idx_t offset_right = 1;
    const idx_t offset_out   = offset_left;

    for (auto [pos_lhs, pos_rhs] : loop(p_lhs, p_rhs, direction)){

        auto i_left  = size_t(flatten<N, StorageOrder::RowMajor>(
            p_lhs.parent_dimensions(), pos_lhs));
        auto i_right = size_t(flatten<N, StorageOrder::RowMajor>(
            p_rhs.parent_dimensions(), pos_rhs));
        /*
        auto i_out  = size_t(flatten<N, StorageOrder::RowMajor>(
            p_rhs.parent_dimensions(), pos_rhs));

        */

        std::array<ET, total_boundary_width> temp;
        
        pick_boundary(&lhs[i_left],
                      &rhs[i_right],
                      temp.begin(),
                      first_idx_to_read,
                      idx_t(total_boundary_width),
                      offset_left,
                      offset_right);


        /*
        for (auto &e : temp){
            //e = 32;
            std::cout << e << std::endl;
        }
        std::exit(1);
        */
        auto out_it = &out[0];
        //auto out_it = &out[i_out] - ((n_boundary_stencils - 1) * offset_out);

        rename_me(temp.cbegin(), out_it, offset_out, n_boundary_stencils, op);


    }
    
}

template <size_t N, class Container, class Op>
static void apply_end(
                  [[maybe_unused]] const Container&    lhs,
                  [[maybe_unused]] const Container&    rhs,
                  [[maybe_unused]] const Partition<N>& p_lhs,
                  [[maybe_unused]] const Partition<N>& p_rhs,
                  [[maybe_unused]] Container&          out,
                  [[maybe_unused]] const position<N>&  direction,
                  [[maybe_unused]] Op op) {

    Utils::runtime_assert(lhs.size() == out.size(), "Array size mismatch.");
    
    using ET        = Container::value_type;
    using tile_t    = Op::Shape;


    //first index that this routine will read relative to the boundary, 
    //can be on either owner (negative) or neighbour side (positive) 
    static constexpr idx_t first_idx_to_read = -tile_t::barrier_end() + 1 + tile_t::get_min();

    //total number of boundary stencils
    static constexpr size_t n_boundary_stencils = size_t(tile_t::barrier_end());

    //total width of the points effected by the stencil (boundary tile witdth)
    static constexpr size_t total_boundary_width = n_boundary_stencils + tile_t::get_width() - 1;


    const idx_t offset_left = 1;
    const idx_t offset_right = 1;
    const idx_t offset_out   = offset_left;

    for (auto [pos_lhs, pos_rhs] : loop(p_lhs, p_rhs, direction)){

        auto i_left  = size_t(flatten<N, StorageOrder::RowMajor>(
            p_lhs.parent_dimensions(), pos_lhs));
        auto i_right = size_t(flatten<N, StorageOrder::RowMajor>(
            p_rhs.parent_dimensions(), pos_rhs));
        auto i_out  = size_t(flatten<N, StorageOrder::RowMajor>(
            p_lhs.parent_dimensions(), pos_lhs));



        std::array<ET, total_boundary_width> temp;

        pick_boundary(&lhs[i_left],
                      &rhs[i_right],
                      temp.begin(),
                      first_idx_to_read,
                      idx_t(total_boundary_width),
                      offset_left,
                      offset_right);


        auto out_it = &out[i_out] - ((n_boundary_stencils - 1) * offset_out);

        rename_me(temp.cbegin(), out_it, offset_out, n_boundary_stencils, op);


    }
    
}


} // namespace JADA
