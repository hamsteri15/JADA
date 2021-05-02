#pragma once
#include <vector>
#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"
#include "containers/md_indexable.hpp"

namespace JADA{


template<size_t N, class T>
struct MdArray : public MdIndexable<N, MdArray<N, T>>{

    using storage_t = std::vector<T>;

    MdArray() = default;


    MdArray(dimension<N> dim) : m_dim(dim), m_data(dim.elementwise_product()) 
    {}

    MdArray(const storage_t& data, dimension<N> dim)
        : m_dim(dim)
        , m_data(data){}


    constexpr const T* get_ptr() const { return m_data.data();}
    constexpr       T* get_ptr()       { return m_data.data(); }
    constexpr dimension<N> const get_dimension() const { return m_dim; }

    const storage_t& get_storage() const {return m_data;}
    storage_t&       get_storage()       {return m_data;}









private:
    dimension<N> m_dim;
    storage_t    m_data;

};


}
