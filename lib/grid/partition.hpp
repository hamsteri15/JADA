#pragma once

#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/md_index_loops.hpp"
#include "loops/flatten_index.hpp"
#include "loops/loopable.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <size_t N> struct Partition : public Loopable<Partition<N>, N> {

    Partition() = default;

    ///
    ///@brief Construct a partition from given dims and a begin position.
    ///
    ///@param parent_dims Dimensions of the parent.
    ///@param begin Begin position of the partition wrt to the parent dimensions.
    ///@param extent Extent of the partition from begin.
    ///
    Partition(dimension<N> parent_dims, position<N> begin, position<N> extent)
        : m_parent_dims(parent_dims)
        , m_begin(begin) 
        , m_extent(extent){
            Utils::runtime_assert(in_bounds(parent_dims, begin, extent), "Partition out of bounds.");
        }


    ///
    ///@brief Construct a partition from a given partition, dimensions and a begin position.
    ///       This is useful for constructing recusive subpartitions.
    ///
    ///@param parent The partition to subset.
    ///@param begin Begin position of the partition wrt to parent dimensions.
    ///@param extent Extent of the new partition from begin.
    ///
    Partition(Partition<N> parent, position<N> begin, position<N> extent) :
    Partition(parent.dimensions(), begin, extent)
    {}


    static bool in_bounds(dimension<N> parent_dims, position<N> begin, position<N> extent){

        return (begin.all_positive() && ((begin + extent) <= parent_dims));

    }

    position<N> loop_begin() const {return m_begin;}

    position<N> loop_end() const {
        return loop_begin() + m_extent;
    }

    dimension<N> parent_dimensions() const {return m_parent_dims;}

    dimension<N> dimensions() const {
        dimension<N> dims;
        for (size_t i = 0; i < N; ++i){
            dims[i] = size_t(m_extent[i]);
        }
        return dims;
    }

    size_t size() const {return size_t(m_extent.elementwise_product());}
    size_t parent_size() const {return m_parent_dims.elementwise_product();}


private:
    dimension<N> m_parent_dims;
    position<N>  m_begin;
    position<N> m_extent;

};


} // namespace JADA