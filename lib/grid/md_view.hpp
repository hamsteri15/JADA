#pragma once

#include "grid/block.hpp"
#include "utils/runtime_assert.hpp"
#include "loops/flatten_index.hpp"
namespace JADA{

template <size_t N, class Container> struct MdView {

    MdView(const Block<N>& block, Container& storage)
        : m_block(block)
        , m_storage(storage)

    {
        Utils::runtime_assert(storage.size() == block.size(),
                              "Storage and block size mismatch");
    }

    const auto& operator()(position<N> pos) const{

        auto idx = size_t(flatten<N, StorageOrder::RowMajor>(m_block.dimensions(), pos));
        return m_storage[idx];
    }

    auto& operator()(position<N> pos){

        auto idx = size_t(flatten<N, StorageOrder::RowMajor>(m_block.dimensions(), pos));
        return m_storage[idx];
    }

    Container& get_storage() {return m_storage;}
    const Container& get_storage() const {return m_storage;}

    const Block<N>& get_block() const {return m_block;}

    size_t size() const {return m_block.size();}


private:
    const Block<N>& m_block;
    Container&      m_storage;
};

}