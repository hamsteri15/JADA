#pragma once

#include <map>
#include "communication/md_communicator_base.hpp"
#include "grid/decomposition.hpp"
#include "grid/block_neighbours.hpp"


namespace JADA{

template<size_t N, class T>
class MockMdCommunicator : public MdCommunicatorBase<MockMdCommunicator<N, T>>{


    //For now
    //using CT = typename ConnectivityType::Star;

    //TODO: make constexpr array of arrays
    //const std::array<std::array<idx_t, N>, 2*N> neighbour_dirs = star_neighbours<N>();


    const BlockNeighbours<N, ConnectivityType::Star> neighbours{};




public:

    MockMdCommunicator() = default;

    MockMdCommunicator(idx_t id, Decomposition<N> decomposition) : 
    m_id(id), m_decomposition(decomposition),
    m_send_buffer(2 * N), m_recv_buffer(2 * N) {}


    bool has_neighbour(position<N> dir) const {
        return m_decomposition.get_neighbour(m_id, dir) != NEIGHBOUR_ID_NULL;
    }


    void set(position<N> dir, T&& data, [[maybe_unused]] size_t tag){

        const auto idx = size_t(neighbours.idx(dir));
        m_send_buffer[idx]  = std::move(data);

    }


    T get(position<N> dir, [[maybe_unused]] size_t tag) {
        const auto idx = size_t(neighbours.idx(dir));
        return m_recv_buffer[idx];
    }


    //TODO: implement a free function to md_communicator_base which calls the sends and receives using set and get

    


private:

    idx_t m_id;

    Decomposition<N> m_decomposition;

    //Could be arrays
    std::vector<T> m_send_buffer;
    std::vector<T> m_recv_buffer;

    
    


};



}