#pragma once

#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"
#include "loops/position.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

// Poor man's implementation of std::experimental::md_span,
// needs to be replaced when compilers start to provide the implementation.
template <size_t N, class Container, StorageOrder SO = StorageOrder::RowMajor>
struct MdView {

    MdView(dimension<N> dim, Container& data)
        : m_dim(dim)
        , m_data(data) {

        Utils::runtime_assert(data.size() == dim.elementwise_product(),
                              "Size mismatch in MdView");
    }

    auto& operator[](position<N> pos) {

        return m_data[static_cast<size_t>(flatten<N, SO>(m_dim, pos))];
    }

    const auto& operator[](position<N> pos) const {

        return m_data[static_cast<size_t>(flatten<N, SO>(m_dim, pos))];
    }

private:
    dimension<N> m_dim;
    Container&   m_data;
};

} // namespace JADA