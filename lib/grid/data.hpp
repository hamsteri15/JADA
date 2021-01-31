#pragma once

#include "grid/block.hpp"
#include "utils/runtime_assert.hpp"
namespace JADA{

template <size_t N, class Container> struct Data {

    Data(const Block<N>& block, Container& storage)
        : m_block(block)
        , m_storage(storage)

    {
        Utils::runtime_assert(storage.size() == block.size(),
                              "Storage and block size mismatch");
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