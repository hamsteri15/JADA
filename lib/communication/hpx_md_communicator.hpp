#pragma once

#include <vector>

#include "communication/md_communicator_base.hpp"
#include <hpx/include/lcos.hpp>

namespace JADA {

template <size_t N, class T, ConnectivityType CT = ConnectivityType::Star>
class HpxMdCommunicator : public MdCommunicatorBase<N, CT> {

public:

    //static constexpr ConnectivityType connectivity_t  = CT;
    using base_type = MdCommunicatorBase<N, CT>;
    using channel_type = hpx::lcos::channel<T>;
    

    HpxMdCommunicator() = default;

    HpxMdCommunicator(idx_t id, Decomposition<N> dec)
        : base_type(id, dec) {

            //auto n_ids = this->get_neighbour_ids();

            auto dirs = this->get_all_neighbour_dirs();

            std::string name = "name";

            for (auto dir : dirs) {

                if (this->has_neighbour(dir)) {
                    [[maybe_unused]] auto idx = this->buffer_idx(dir);

                    
                    [[maybe_unused]] auto recv_id = size_t(this->neigbour_id(dir));

                    m_recv[idx] = hpx::find_from_basename<channel_type>(name, recv_id);
                    
//                    auto temp = hpx::find_here();                    

                    auto temp = hpx::lcos::channel<T>(hpx::find_here());

                    //m_send[idx] = channel_type(this->id());
                    
                    //m_send[idx] = channel_type(hpx::find_here());
                    
                    //hpx::register_with_basename(name, m_send[idx], size_t(this->id()));
                    


                }

                




            }



        }

private:

    std::vector<channel_type> m_send;
    std::vector<channel_type> m_recv;

    /*
    std::vector<T> m_send_buffer;
    std::vector<T> m_recv_buffer;
    */


};

} // namespace JADA