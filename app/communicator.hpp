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


            static const char* basename = "stencil";

            for (direction<N> dir : m_neighbours.get()){

                if (dec.get_neighbour(id, dir) != NEIGHBOUR_ID_NULL){

                    
                    //size_t idx = 0;


                    //m_recv[size_t(m_neighbours.idx(dir))] = 
                    m_recv[size_t(0)] = 
                                        hpx::find_from_basename<channel_type>(
                                                basename, 
                                                size_t(dec.get_neighbour(id, dir))
                                                );




                    //auto idx2 = size_t(m_neighbours.idx(dir));
                    m_send[size_t(0)] = channel_type(hpx::find_here());
                    hpx::register_with_basename(basename, m_send[size_t(0)], size_t(id));
                    /*
                    m_send[idx2] = channel_type(hpx::find_here());
                    hpx::register_with_basename(basename, m_send[idx2], size_t(id));
                    */

                    std::cout << "taalla " << id <<" " << dir<< std::endl;
                    


                }
            }

            /*
             // Retrieve the channel from our upper neighbor from which we receive
                // the row we need to update the first row in our partition.
                recv[up] = hpx::find_from_basename<channel_type>(down_name, rank - 1);

                // Create the channel we use to send our first row to our upper
                // neighbor
                send[up] = channel_type(hpx::find_here());
                // Register the channel with a name such that our neighbor can find it.
                hpx::register_with_basename(up_name, send[up], rank);

            */

            /*

            // Retrieve the channel from our neighbor below from which we receive
                // the row we need to update the last row in our partition.
                recv[down] = hpx::find_from_basename<channel_type>(up_name, rank + 1);
                // Create the channel we use to send our last row to our neighbor
                // below
                send[down] = channel_type(hpx::find_here());
                // Register the channel with a name such that our neighbor can find it.
                hpx::register_with_basename(down_name, send[down], rank);

            */

        }
    }

    
    void set(direction<N> dir, T&& t, std::size_t step)
    {
        // Send our data to the neighbor n using fire and forget semantics
        // Synchronization happens when receiving values.
        size_t n = size_t(m_neighbours.idx(dir)) * 0;
        m_send[n].set(hpx::launch::apply, std::move(t), step);
    }
    


    hpx::future<T> get(direction<N> dir, std::size_t step)
    {
        // Get our data from our neighbor, we return a future to allow the
        // algorithm to synchronize.
        size_t n = size_t(m_neighbours.idx(dir)) * 0;
        return m_recv[n].get(hpx::launch::async, step);
    }
    



        /*

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
        */

    



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
