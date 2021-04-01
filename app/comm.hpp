#pragma once

#include "loops/index_type.hpp"
#include "communication/md_communicator.hpp"
#include "grid/decomposition.hpp"

using namespace JADA;


struct Comm : public MdCommunicatorBase<2, ConnectivityType::Box>{


    Comm(idx_t id, Decomposition<2> dec) : 
    MdCommunicatorBase(id, dec) {}



    template<class global_t>
    auto get_neighbour_data(global_t global, position<2> dir) const {
        auto id = neigbour_id(dir);
        return global.at(size_t(id));
    }

private:
  
};


struct MockComm{

    MockComm(Decomposition<2> dec) : 
    m_comms(create_comms(dec)) {} 
    

    auto get_local_comm(idx_t id) const{
        return m_comms.at(size_t(id));
    }

    auto get_comms() const {
        return m_comms;
    }



private:
    std::vector<Comm> m_comms;
    
    static std::vector<Comm> create_comms(Decomposition<2> dec) {

        size_t n_partitions = dec.dimensions().elementwise_product();
        std::vector<Comm> ret;
        for (idx_t id = 0; id < idx_t(n_partitions); ++id) {
            ret.push_back(
                Comm(id, dec)
            );
        }

        return ret;
    }


};