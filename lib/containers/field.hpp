#pragma once

#include <cstddef>
#include <hpx/include/partitioned_vector.hpp>
#include <hpx/runtime_distributed/find_localities.hpp>
#include <hpx/algorithm.hpp>

#include "loops/dimension.hpp"

namespace JADA{

template<size_t N, class ET>
struct Field{


    using element_t = ET;
    using storage_t = hpx::partitioned_vector<element_t>;

    Field(dimension<N> dims) : m_dims(dims), m_storage(create_storage(dims)) {}


    Field& operator=(element_t rhs) {

        hpx::fill(m_storage.begin(), m_storage.end(), rhs);
        return *this;
    }

    auto begin() {return m_storage.begin();}
    auto end()   {return m_storage.end();}

    const auto& get_storage() const {return m_storage;}




private:
    dimension<N> m_dims;
    storage_t    m_storage;    
    

    static size_t compute_segments(dimension<N> dims) {

        return dims.elementwise_product() / *dims.end();

    }

    static storage_t create_storage(dimension<N> dims) {

        auto n_segments = compute_segments(dims);
        auto locs = hpx::find_all_localities();
        auto layout = hpx::container_layout( n_segments, locs );
        auto n_elements = dims.elementwise_product();
        return storage_t(n_elements);

    }

};



}

