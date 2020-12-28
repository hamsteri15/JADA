#pragma once

#include "grid/boundary.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"
#include "loops/loopable.hpp"
#include "loops/md_index_loops.hpp"
#include "loops/paired_md_index_loops.hpp"
#include "loops/position.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <size_t N> struct SubBlock;

template <size_t N> struct Block : public Loopable<Block<N>, N> {

    Block() = default;

    Block(position<N> begin, position<N> extent)
        : m_begin(begin)
        , m_extent(extent) {
            Utils::runtime_assert(extent.elementwise_product() != 0, "Zero size block." );
        }

    size_t      size() const { return size_t(m_extent.elementwise_product()); }
    position<N> begin() const { return m_begin; }
    position<N> end() const { return m_begin + m_extent; }

    position<N> extent() const { return m_extent; }
    position<N> loop_begin() const { return begin(); }
    position<N> loop_end() const { return end(); }

    dimension<N> dimensions() const {
        dimension<N> dims;
        for (size_t i = 0; i < N; ++i) { dims[i] = size_t(m_extent[i]); }
        return dims;
    }

    Boundary<N> get_boundary(position<N> direction) const {
        return Boundary<N>(dimensions(), direction, m_begin);
    }

    bool contains(position<N> pos) const {
        return (pos >= begin()) && (pos < end());
    }

    SubBlock<N> get_subblock(position<N> begin, position<N> extent) const {
        return SubBlock<N>(*this, begin, extent);
    }

private:
    position<N> m_begin;
    position<N> m_extent;
};

template <size_t N> struct SubBlock : public Block<N> {

    SubBlock(const Block<N>& parent, position<N> begin, position<N> extent)
        : Block<N>(begin, extent)
        , m_parent(parent) {
        Utils::runtime_assert(in_bounds(parent.dimensions(), begin, extent),
                              "SubBlock out of bounds.");
    }

    bool shares_boundary(const SubBlock<N>& rhs);

private:
    const Block<N>& m_parent;

    static bool
    in_bounds(dimension<N> dims, position<N> begin, position<N> extent) {
        return (begin.all_positive() && ((begin + extent) <= dims));
    }
};




///
///@brief Loops over the positions shared by partition owner and neighbour. Usage:
///         for (auto [p1, p2] : loop(owner_partition, neighbour_partition, {0,1})){}
///
///@tparam N 
///@param lhs The lhs partition, the index in the lhs partition is returned as the first element 
///@param rhs The rhs partition, the index in the rhs partition is returned as the second element
///@param direction The direction of the interface between the two partitions
///@return auto Sequence of position pairs [i_owner, i_neighbour] 
///
template<size_t N>
auto loop(const Block<N>& lhs, const Block<N>& rhs, position<N> direction){

    auto bo = lhs.get_boundary(direction);
    auto bn = rhs.get_boundary(-direction);
    auto extent = bo.loop_end() - bo.loop_begin();

    return paired_md_indices(bo.loop_begin(), bn.loop_begin(), extent);

}

} // namespace JADA
