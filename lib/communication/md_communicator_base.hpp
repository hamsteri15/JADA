#pragma once

#include "communication/communicator.hpp"
#include "grid/block_neighbours.hpp"
#include "grid/decomposition.hpp"
#include "loops/position.hpp"

namespace JADA{

template<size_t N, ConnectivityType CT> 
class MdCommunicatorBase{




public:

    MdCommunicatorBase() = default;

    MdCommunicatorBase(idx_t id, Decomposition<N> dec) : m_id(id), m_decomposition(dec) {}



//CRTP injected functions
    
    /*
    
    Derived* derived() {
        return static_cast<Derived*>(this);
    }

    const Derived* derived() const {
        return static_cast<const Derived*>(this);
    }


    //Send data to direction dir
    template<class T>
    void set(position<N> dir, T&& data, size_t tag){
        derived().set(dir, data, tag);
    }


    //Get data from direction dir
    auto get(position<N> dir, size_t tag) {
        return derived().get(dir, tag);
    }


    */


    ///
    ///@brief Checks if a neighbour exists in direction dir
    ///
    ///@param dir direction to query the neighbour from
    ///@return true if exists
    ///@return false otherwise
    ///
    bool has_neighbour(position<N> dir) const {
        return m_decomposition.get_neighbour(m_id, dir) != NEIGHBOUR_ID_NULL;
    }



    



protected:

    idx_t             m_id;
    Decomposition<N>  m_decomposition;
    static constexpr BlockNeighbours<N, CT> neighbours{};



};




}