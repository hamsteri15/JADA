#pragma once

#include <cstddef>
#include <hpx/algorithm.hpp>
#include <hpx/include/partitioned_vector.hpp>
#include <hpx/runtime_distributed/find_localities.hpp>

#include "loops/dimension.hpp"

namespace JADA {


template<class T>
static auto local_begin(hpx::partitioned_vector<T> v) {
    return v.begin();
} 

template<class T>
static auto local_end(hpx::partitioned_vector<T> v) {
    return v.end();
} 








template <size_t N>
static size_t segment_count(dimension<N> dims) {

    if constexpr (N == 1) {
        return size_t(1);
    }

    return dims.elementwise_product() / dims.back();
}

template <class T> 
static size_t segment_count(hpx::partitioned_vector<T> v) {

    return size_t(v.segment_cend() - v.segment_cbegin());
}




template<size_t N, class T>
static hpx::partitioned_vector<T> make_partitioned_vector(dimension<N> dims) {

    auto num_segments = segment_count(dims);

    std::vector<hpx::id_type> locs = hpx::find_all_localities();

    auto layout = hpx::container_layout( num_segments, locs );

    return hpx::partitioned_vector<T>(dims.elementwise_product(), layout);
}


template<class T>
static auto get_segment(hpx::partitioned_vector<T> v, size_t seg_n) {

    Utils::runtime_assert(seg_n < segment_count(v), "Segment out of bounds.");

    using iterator = hpx::partitioned_vector<T>::iterator;
    using traits   = hpx::traits::segmented_iterator_traits<iterator>;

    auto seg_it = traits::segment(v.begin()) + std::ptrdiff_t(seg_n);


    auto loc_begin = traits::begin(seg_it);
    auto loc_end   = traits::end(seg_it);

    return std::make_pair(loc_begin, loc_end);

}

template<class T>
static std::vector<size_t> segment_sizes(hpx::partitioned_vector<T> v) {

    size_t n = segment_count(v);

    std::vector<size_t> ret(n);

    for (size_t i = 0; i < n; ++i) {
        auto [begin, end] = get_segment(v, i);
        ret[i] = size_t(std::distance(begin, end));
    }
    return ret;


}

template<class T>
static bool uniform_segments(hpx::partitioned_vector<T> v) {

    auto sizes = segment_sizes(v);
    return std::equal(sizes.begin(), sizes.end(), sizes.begin());

}

template<class T>
static size_t local_segment_count(hpx::partitioned_vector<T> v, uint32_t locality_id) {

    auto local_sbegin = v.segment_begin(locality_id);
    auto local_send = v.segment_end(locality_id);
    return size_t(std::distance(local_sbegin, local_send));
        
}

template<class T>
static size_t local_segment_count(hpx::partitioned_vector<T> v) {

    return local_segment_count(v, hpx::get_locality_id());
        
}

template<class T>
static size_t local_first_segment_number(hpx::partitioned_vector<T> v, uint32_t locality_id) {

    auto local_sbegin = v.segment_cbegin(locality_id);
    return size_t(v.get_partition(local_sbegin));
}



template<class T>
static size_t local_first_segment_number(hpx::partitioned_vector<T> v) {

    return local_first_segment_number(v, hpx::get_locality_id());
}


template<class T>
static size_t local_last_segment_number(hpx::partitioned_vector<T> v, uint32_t locality_id) {

    auto local_send = v.segment_cend(locality_id);
    return size_t(v.get_partition(local_send));
}


template<class T>
static size_t local_last_segment_number(hpx::partitioned_vector<T> v) {

    return local_last_segment_number(v, hpx::get_locality_id());
}






} // namespace JADA