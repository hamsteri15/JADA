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

template <size_t N, class Container> struct SplitData {

    SplitData(Data<N, Container> data_owner,
              Data<N, Container> data_neigh,
              position<N>        direction)
        : m_data_owner(data_owner)
        , m_data_neigh(data_neigh)
        , m_direction(direction) {

        Utils::runtime_assert(dimensions_match(data_owner.get_block(),
                                               data_neigh.get_block(),
                                               direction),
                              "Blocks are not contiguous");
    }

    SplitData(const Block<N>& o_block,
              const Block<N>& n_block,
              Container&      o_storage,
              Container&      n_storage,
              position<N>     direction)
        : SplitData(Data<N, Container>(o_block, o_storage),
                    Data<N, Container>(n_block, n_storage),
                    direction) {}

private:
    Data<N, Container> m_data_owner;
    Data<N, Container> m_data_neigh;
    position<N>        m_direction;

    static bool dimensions_match(const Block<N>& b1,
                                 const Block<N>& b2,
                                 position<N>     direction) {

        auto dims1 = b1.dimensions();
        auto dims2 = b2.dimensions();

        for (size_t i = 0; i < N; ++i) {
            if ((direction[i] == 0) && (dims1[i] != dims2[i])) { return false; }
        }
        return true;
    }
};
}