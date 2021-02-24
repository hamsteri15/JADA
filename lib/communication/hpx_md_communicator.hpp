#pragma once

#include <vector>

#include "communication/md_communicator_base.hpp"
#include <hpx/include/lcos.hpp>

namespace JADA {

template <size_t N, class T, ConnectivityType CT = ConnectivityType::Star>
class HpxMdCommunicator : public MdCommunicatorBase<N, CT, HpxMdCommunicator<N, T, CT>> {

public:


    using base_type = MdCommunicatorBase<N, CT, HpxMdCommunicator<N,T,CT>>;
    using channel_type = hpx::lcos::channel<T>;
    

    HpxMdCommunicator() = default;

    HpxMdCommunicator(idx_t id, Decomposition<N> dec) : 
    base_type(id, dec) {}


private:

    std::vector<T> m_send_buffer;
    std::vector<T> m_recv_buffer;



};

} // namespace JADA