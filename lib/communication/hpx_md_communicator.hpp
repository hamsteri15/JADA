#pragma once

#include <vector>

#include "utils/runtime_assert.hpp"
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
        : base_type(id, dec), m_send(this->neighbour_count()), m_recv(this->neighbour_count()) {


            auto dirs = this->get_all_neighbour_dirs();

            //std::string name = "name";

            for (auto dir : dirs) {

                auto name = to_string(dir);

                if (this->has_neighbour(dir)) {
                    

                    auto idx = this->buffer_idx(dir);

                    auto n_id = size_t(this->neigbour_id(dir));

                    m_recv[idx] = hpx::find_from_basename<channel_type>(name, n_id);
                    m_send[idx] = channel_type(hpx::find_here());
                    hpx::register_with_basename(name, m_send[idx], size_t(this->id()));

                }

            }

        }


    static std::string to_string(position<N> dir) {
        std::ostringstream oss;
        std::copy(dir.begin(), dir.end()-1,
        std::ostream_iterator<idx_t>(oss, ","));

    // Now add the last element with no delimiter
        oss << dir[N-1];
        return oss.str();
    }


    void set(position<N> dir, T&& t, size_t tag)
    {
        // Send our data to the neighbor n using fire and forget semantics
        // Synchronization happens when receiving values.
        //Utils::runtime_assert(this->has_neighbour(dir), "Trying to set for invalid neighbour");

            auto idx = this->buffer_idx(dir);

            m_send[idx].set(hpx::launch::apply, std::move(t), tag);


    }

    
    hpx::future<T> get(position<N> dir, size_t tag)
    {
        // Get our data from our neighbor, we return a future to allow the
        // algorithm to synchronize.
        //Utils::runtime_assert(this->has_neighbour(dir), "Trying to get from invalid neighbour");


            auto idx = this->buffer_idx(dir);
            return m_recv[idx].get(hpx::launch::async, tag);
    }
    











private:

    //TODO: could be arrays
    std::vector<channel_type> m_send;
    std::vector<channel_type> m_recv;

};

} // namespace JADA