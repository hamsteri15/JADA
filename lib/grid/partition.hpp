#pragma once

#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/md_index_loops.hpp"
#include "loops/flatten_index.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <size_t N> struct Partition {

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


    position<N> global_begin() const {
        return m_begin;
    }
    position<N> global_end() const {
        position<N> end;
        for (size_t i = 0; i < N; ++i){
            end[i] = m_begin[i] + idx_t(m_dims[i]);
        }
        return end;
    }


    index_generator<N> global_md_indices() const{
        return md_indices(global_begin(), global_end());
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
template<size_t N>
index_generator<2> partition_loop(Partition<N> p) {

    for (auto pos : p.global_md_indices()){

        auto partition_pos = pos - p.global_begin();
        auto partition_i = flatten<N, StorageOrder::RowMajor>(p.dimensions(), partition_pos);
        auto data_i = flatten<N, StorageOrder::RowMajor>(p.parent_dimensions(), pos);

        co_yield {partition_i, data_i}; 
    }
}

template<size_t N, class Container>
Container subset_data(Partition<N> p, const Container& data){

    Utils::runtime_assert(data.size() == p.parent_size(), "Partition parent size differs from container size");
    Container ret(p.size());
    
    for (auto [p_i, d_i] : partition_loop(p)){
        ret[size_t(p_i)] = data[size_t(d_i)];
    }

    return ret;

}





template<size_t N, class T>
void apply_operation(Partition<N> owner, Partition<N> neighbour, T& owner_data, T& neighbour_data){

    Utils::runtime_assert(owner.parent_dimensions() == neighbour.parent_dimensions(), "Partitions dont share the same parent.");
    Utils::runtime_assert(owner.size() == neighbour.size(), "Partition size mismatch.");


    for (auto pos : owner.global_md_indices()){

        auto n_pos = pos - owner.global_begin();

        auto owner_i = flatten<N, StorageOrder::RowMajor>(owner.parent_dimensions(), pos);
        auto neighbour_i = flatten<N, StorageOrder::RowMajor>(neighbour.parent_dimensions(), n_pos);

        neighbour_data[size_t(neighbour_i)] = owner_data[size_t(owner_i)];

    }
   

}



} // namespace JADA