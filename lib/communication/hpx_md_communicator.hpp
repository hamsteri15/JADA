#pragma once

#include <vector>

#include "communication/md_communicator.hpp"
#include <hpx/include/lcos.hpp>

namespace JADA {

template <typename T, size_t N, ConnectivityType CT>
class HpxMdCommunicator : public MdCommunicator<N, CT> {

public:
    
    using base_type = MdCommunicator<N, CT>;
    using channel_type = hpx::lcos::channel<T>;
    


    HpxMdCommunicator() = default;

    HpxMdCommunicator(idx_t id, Decomposition<N> dec)
        : MdCommunicator<N, CT>(id, dec) {

        for (direction<N> dir : this->get_directions()){

            if (this->has_neighbour(dir)) {

                std::ostringstream stream1;
                stream1 << dir;
                std::string basename_send = stream1.str();

                m_recv[this->neighbour_idx(dir)] = hpx::find_from_basename<channel_type>(
                    basename_send, this->get_neighbour(dir));

                std::ostringstream stream2;
                stream2 << -dir;
                std::string basename_recv = stream2.str();

                m_send[this->neighbour_idx(dir)] = channel_type(hpx::find_here());
                hpx::register_with_basename(
                    basename_recv, m_send[this->neighbour_idx(dir)], size_t(id));
            }
        }
    }


    void set(direction<N> dir, T&& t, std::size_t step) {
        m_send[this->neighbour_idx(dir)].set(hpx::launch::apply, std::move(t), step);
    }

    hpx::future<T> get(direction<N> dir, std::size_t step) {
        return m_recv[this->neighbour_idx(dir)].get(hpx::launch::async, step);
    }


private:
    std::array<channel_type, base_type::neighbour_count()> m_recv;
    std::array<channel_type, base_type::neighbour_count()> m_send;

};


} // namespace JADA