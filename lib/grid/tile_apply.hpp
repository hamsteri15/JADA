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
void pick_boundary(It left, It right, OutIt out, idx_t first, idx_t last, idx_t offset_left, idx_t offset_right){


    for (idx_t i = first; i < last; ++i, ++out) {

        if (i <= 0) {
            auto it = left + i * offset_left;
            *out       = *it;

        } else {
            auto it = right + (offset_right * (i-1));
            *out       = *it;
        }
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




    //first index that this routine will compute relative to the boundary, always on owner side and max -0
    static constexpr idx_t first_computable_idx = -tile_t::barrier_end() + 1;

    //first index that this routine will read relative to the boundary, 
    //can be on either owner (negative) or neighbour side (positive) 
    static constexpr idx_t first_idx_to_read = first_computable_idx + tile_t::get_min();


    //total number of boundary stencils
    [[maybe_unused]] static constexpr size_t n_boundary_stencils = size_t(tile_t::barrier_end());


    //total width of the points effected by the stencil (boundary tile witdth)
    [[maybe_unused]] static constexpr size_t total_boundary_width = n_boundary_stencils + tile_t::get_width() - 1;


    //last index that this routine will read relative to the boundary, 
    //can be on either owner (negative) or neighbour side (positive)
    static constexpr idx_t  last_idx_to_read = first_idx_to_read + idx_t(total_boundary_width);

    const idx_t offset_left = 1;
    const idx_t offset_right = 1;
    const idx_t offset_out   = offset_left;

    /*
    std::cout << first_computable_idx << std::endl;
    std::cout << first_idx_to_read << std::endl;
    std::cout << n_boundary_stencils << std::endl;
    std::cout << total_boundary_width << std::endl;
    std::cout << last_idx_to_read << std::endl;
    std::cout << tile_t::get_width() << std::endl;
    std::exit(1);
    */
    for (auto [p_owner, p_neigh] : loop(p1, p2, direction)){

        auto i_left  = size_t(flatten<N, StorageOrder::RowMajor>(
            p1.parent_dimensions(), p_owner));
        auto i_right = size_t(flatten<N, StorageOrder::RowMajor>(
            p2.parent_dimensions(), p_neigh));
        auto i_out  = size_t(flatten<N, StorageOrder::RowMajor>(
            p1.parent_dimensions(), p_owner));



        std::array<ET, total_boundary_width> temp;

        pick_boundary(&in1[i_left],
                      &in2[i_right],
                      temp.begin(),
                      first_idx_to_read,
                      last_idx_to_read,
                      offset_left,
                      offset_right);

        /*for (auto t : temp){
            std::cout << t << std::endl;
        }*/

        auto out_it = &out[i_out] - ((n_boundary_stencils - 1) * offset_out);

        for (size_t i = 0; i < n_boundary_stencils; ++i, out_it += offset_out) {

            storage_t s(std::cbegin(temp) + i,
                        std::cbegin(temp) + i + tile_t::get_width());

            //auto sol = Op::apply(s);

            s.print();
            std::cout << s(1) << std::endl; //+ s(2) << std::endl;
            std::cout << s(2) << std::endl; //+ s(2) << std::endl;
            //s.print();
            //std::cout << sol << std::endl;

            *out_it = Op::apply(s);
        }
    }
    
}


} // namespace JADA
