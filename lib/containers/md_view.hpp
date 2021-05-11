#pragma once

#include "containers/md_indexable.hpp"
#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {


// Poor man's implementation of std::experimental::md_span,
// needs to be replaced when compilers start to provide the implementation.
template <size_t N, class Container>
struct MdView : public MdIndexable<N, MdView<N, Container>>{


    using value_type = typename Container::value_type;


    MdView(dimension<N> dim, Container& data)
        : m_dim(dim)
        , m_data(data) {

        Utils::runtime_assert(data.size() == dim.elementwise_product(),
                              "Size mismatch in MdView");
    }


    constexpr const auto* get_ptr() const { return m_data.data();}
    constexpr       auto* get_ptr()       { return m_data.data(); }
    constexpr dimension<N> const get_dimension() const { return m_dim; }


private:
    dimension<N> m_dim;
    Container&   m_data;
};

} // namespace JADA