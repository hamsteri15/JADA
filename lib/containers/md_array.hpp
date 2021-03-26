#pragma once
#include <vector>
#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"

namespace JADA{

template<size_t N, class T, StorageOrder SO = StorageOrder::RowMajor>
struct MdArray{

    using storage_t = std::vector<T>;

    MdArray() = default;


    MdArray(dimension<N> dim) : m_dim(dim), m_data(dim.elementwise_product()) 
    {}

    size_t size() const {
        return m_data.size();
    }

    T& operator[](position<N> pos) {

        return m_data[static_cast<size_t>(flatten<N, SO>(m_dim, pos))];
    }

    const T& operator[](position<N> pos) const {

        return m_data[static_cast<size_t>(flatten<N, SO>(m_dim, pos))];
    }


private:

    dimension<N> m_dim;
    storage_t    m_data;

};


}
