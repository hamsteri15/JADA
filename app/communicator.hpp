#pragma once



#include <hpx/include/lcos.hpp>

#include <array>

#include "loops/direction.hpp"
#include "grid/neighbours.hpp"
#include "grid/decomposition.hpp"


namespace JADA {


template<typename T, size_t N = 2>
struct communicator_own
{
    using channel_type = hpx::lcos::channel<T>;

    communicator_own() = default;


    communicator_own(idx_t id, Decomposition<N> dec) : 
    m_id(id), m_dec(dec){

        if (dec.dimensions().elementwise_product() > 0){



            for (direction<N> dir : m_neighbours.get()){

                if (dec.get_neighbour(id, dir) != NEIGHBOUR_ID_NULL){

                    
                    std::ostringstream stream1;
                    stream1 << dir;
                    std::string basename_send =  stream1.str();
                    
                    
                    m_recv[recv_idx(dir)] = 
                                        hpx::find_from_basename<channel_type>(
                                                basename_send, 
                                                size_t(dec.get_neighbour(id, dir))
                                                );

                    std::ostringstream stream2;
                    stream2 << -dir;
                    std::string basename_recv =  stream2.str();



                    m_send[send_idx(dir)] = channel_type(hpx::find_here());
                    hpx::register_with_basename(basename_recv, m_send[send_idx(dir)], size_t(id));

                }
            }

        }
    }

    size_t send_idx(direction<N> dir) const {
        return size_t(m_neighbours.idx(dir));
    }

    size_t recv_idx(direction<N> dir) const {      
        //return size_t(m_neighbours.idx(-dir));
        return size_t(m_neighbours.idx(dir));
    }

    idx_t id() const {return m_id;}


    void set(direction<N> dir, T&& t, std::size_t step)
    {
        m_send[send_idx(dir)].set(hpx::launch::apply, std::move(t), step);
    }
    


    hpx::future<T> get(direction<N> dir, std::size_t step)
    {
        return m_recv[recv_idx(dir)].get(hpx::launch::async, step);
    }
    


    static constexpr auto get_dirs() {
        return m_neighbours.get();
    }


    bool has_neighbour(direction<N> dir) const {

        return m_dec.get_neighbour(m_id, dir) != NEIGHBOUR_ID_NULL;

    }

    size_t get_neighbour(direction<N> dir) const {
        
        Utils::runtime_assert(has_neighbour(dir), "Trying to get invalid neighbour");
        return size_t(m_dec.get_neighbour(m_id, dir));

    }

       
private:

    static constexpr Neighbours<N, ConnectivityType::Box> m_neighbours{};

    static constexpr size_t m_neighbour_count = m_neighbours.count();

    idx_t m_id;
    Decomposition<2> m_dec;
    std::array<channel_type, m_neighbour_count> m_recv;
    std::array<channel_type, m_neighbour_count> m_send;


};

}

template <typename T>
struct communicator
{
    enum neighbor {
        up = 0,
        down = 1,
    };

    using channel_type = hpx::lcos::channel<T>;


    // rank: our rank in the system
    // num: number of participating partners
    communicator(std::size_t rank, std::size_t num)
    {
        static const char* up_name = "/stencil/up/";
        static const char* down_name = "/stencil/down/";

        // Only set up channels if we have more than one partner
        if (num > 1)
        {
            // We have an upper neighbor if our rank is greater than zero.
            if (rank > 0)
            {
                // Retrieve the channel from our upper neighbor from which we receive
                // the row we need to update the first row in our partition.
                recv[up] = hpx::find_from_basename<channel_type>(down_name, rank - 1);

                // Create the channel we use to send our first row to our upper
                // neighbor
                send[up] = channel_type(hpx::find_here());
                // Register the channel with a name such that our neighbor can find it.
                hpx::register_with_basename(up_name, send[up], rank);
            }
            if (rank < num - 1)
            {
                // Retrieve the channel from our neighbor below from which we receive
                // the row we need to update the last row in our partition.
                recv[down] = hpx::find_from_basename<channel_type>(up_name, rank + 1);
                // Create the channel we use to send our last row to our neighbor
                // below
                send[down] = channel_type(hpx::find_here());
                // Register the channel with a name such that our neighbor can find it.
                hpx::register_with_basename(down_name, send[down], rank);
            }
        }
    }

    bool has_neighbor(neighbor n) const
    {
        return recv[n] && send[n];
    }

    void set(neighbor n, T&& t, std::size_t step)
    {
        // Send our data to the neighbor n using fire and forget semantics
        // Synchronization happens when receiving values.
        send[n].set(hpx::launch::apply, std::move(t), step);
    }

    hpx::future<T> get(neighbor n, std::size_t step)
    {
        // Get our data from our neighbor, we return a future to allow the
        // algorithm to synchronize.
        return recv[n].get(hpx::launch::async, step);
    }

    std::array<hpx::lcos::channel<T>, 2> recv;
    std::array<hpx::lcos::channel<T>, 2> send;
};
