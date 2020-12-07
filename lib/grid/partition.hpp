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
    ///@param dims Dimensions of the partition.
    ///@param begin Begin position of the partition wrt to the parent dimensions.
    ///
    Partition(dimension<N> parent_dims, dimension<N> dims, position<N> begin)
        : m_parent_dims(parent_dims)
        , m_dims(dims)
        , m_begin(begin) {
            Utils::runtime_assert(in_bounds(parent_dims, dims, begin), "Partition out of bounds.");
        }


    ///
    ///@brief Construct a partition from a given partition, dimensions and a begin position.
    ///       This is useful for constructing recusive subpartitions.
    ///
    ///@param parent The partition to subset.
    ///@param dims Dimensions of the new partition
    ///@param begin Begin position of the partition wrt to parent dimensions.
    ///
    Partition(Partition<N> parent, dimension<N> dims, position<N> begin) :
    Partition(parent.dimensions(), dims, begin)
    {}


    static bool in_bounds(dimension<N> parent_dims, dimension<N> dims, position<N> begin){

        for (size_t i = 0; i < N; ++i){
            if (begin[i] < 0){
                return false;
            }
            if (begin[i] + idx_t(dims[i]) > idx_t(parent_dims[i])){
                return false;
            }
        }
        return true;
    }

    position<N> loop_begin() const {return m_begin;}

    position<N> loop_end() const {
        position<N> end;
        for (size_t i = 0; i < N; ++i){
            end[i] = m_begin[i] + idx_t(m_dims[i]);
        }
        return end;
    
    }

    dimension<N> parent_dimensions() const {return m_parent_dims;}
    dimension<N> dimensions() const {return m_dims;}

    size_t size() const {return m_dims.elementwise_product();}
    size_t parent_size() const {return m_parent_dims.elementwise_product();}


private:
    dimension<N> m_parent_dims;
    dimension<N> m_dims;
    position<N>  m_begin;

};


} // namespace JADA