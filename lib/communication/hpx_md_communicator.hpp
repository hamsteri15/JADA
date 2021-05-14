#pragma once

#include <vector>

#include <hpx/include/lcos.hpp>
#include "communication/md_communicator.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <typename T, size_t N, ConnectivityType CT>
class HpxMdCommunicator : public MdCommunicator<N, CT> {

public:
    
    using base_type = MdCommunicator<N, CT>;
    using channel_type = hpx::lcos::channel<T>;
    


    HpxMdCommunicator() = default;

    HpxMdCommunicator(std::string name, idx_t id, Decomposition<N> dec)
        : MdCommunicator<N, CT>(id, dec) {

            m_name = name;

        for (direction<N> dir : this->get_directions()){

            if (this->has_neighbour(dir)) {


                m_recv[recv_idx(dir)] = hpx::find_from_basename<channel_type>(
                    name + send_name(dir), this->get_neighbour(dir));

                
                m_send[send_idx(dir)] = channel_type(hpx::find_here());
                hpx::register_with_basename(
                    name + recv_name(dir), m_send[send_idx(dir)], size_t(id));
            }
        }
    }


    void set(direction<N> dir, T&& t, std::size_t step) {
        Utils::runtime_assert(this->has_neighbour(dir), "Trying to set for non-existing neighbour.");
        m_send[send_idx(dir)].set(hpx::launch::apply, std::move(t), step);
    }

    hpx::future<T> get(direction<N> dir, std::size_t step) {
        Utils::runtime_assert(this->has_neighbour(dir), "Trying to get from non-existing neighbour.");
        return m_recv[recv_idx(dir)].get(hpx::launch::async, step);
    }


    hpx::shared_future<T> get_shared(direction<N> dir, std::size_t step) {
        Utils::runtime_assert(this->has_neighbour(dir), "Trying to get from non-existing neighbour.");
        return m_recv[recv_idx(dir)].get(hpx::launch::async, step);
    }



private:
    std::string m_name; 
    std::array<channel_type, base_type::neighbour_count()> m_recv;
    std::array<channel_type, base_type::neighbour_count()> m_send;

    static std::string send_name(direction<N> dir) {
        std::ostringstream stream;
        stream << -dir;
        return stream.str();
    }

    static std::string recv_name(direction<N> dir) {
        std::ostringstream stream;
        stream << dir; //neg
        return stream.str();
    }

    static constexpr size_t send_idx(direction<N> dir) {
        return base_type::neighbour_idx(dir);
    }

    static constexpr size_t recv_idx(direction<N> dir) {
        return base_type::neighbour_idx(dir); //neg
    }


};


} // namespace JADA