#include <hpx/algorithm.hpp>
#include <hpx/chrono.hpp>
#include <hpx/execution.hpp>
#include <hpx/future.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/compute.hpp>
#include <hpx/include/util.hpp>
#include <hpx/init.hpp>
#include <hpx/program_options.hpp>



#include <iostream>
#include <vector>
#include <utility>
#include <numeric>
#include <array>

#include "containers/md_array.hpp"

#include "communication/hpx_md_communicator.hpp"

using namespace JADA;


//using communication_type = MdArray<2, double>; //this would require a serialization method
using communication_type = std::vector<double>;
HPX_REGISTER_CHANNEL_DECLARATION(communication_type)
HPX_REGISTER_CHANNEL(communication_type, stencil_communication)

//using own_communicator_type = communicator_own<communication_type>;

using own_communicator_type = HpxMdCommunicator<communication_type, 2, ConnectivityType::Box>;


template<class T>
void print(std::vector<T> v) {

    std::cout << "{ ";
    for (auto vv : v) {
        std::cout << vv << std::endl;
    }
    std::cout << "}";


}


int hpx_main(){


    size_t Nx = 10;
    size_t Ny = 10;


    std::size_t num_local_partitions = 4;


    Decomposition<2> dec(
        {Ny, Nx},
        num_local_partitions,
        {false, false}
    );

    std::vector<own_communicator_type> comms;

    for (idx_t i = 0; i < idx_t(num_local_partitions); ++i){

        comms.push_back(
            own_communicator_type(i, dec)
        );
    }    


    auto all_dirs = comms[0].get_directions();

    
    for (auto& comm : comms){

        for (direction<2> dir : all_dirs) {
            if (comm.has_neighbour(dir)){
                //std::cout << "Id: " << comm.id() << "  set: " << dir << std::endl;
                std::vector<double> send_data(1, double(comm.id()));
                comm.set(dir, std::move(send_data), 0);
            }
        }

    }


    for (auto& comm : comms) {
        for (direction<2> dir : all_dirs) {

            if (comm.has_neighbour(dir)){
                auto tt = comm.get(dir, 0).get();
                std::cout << comm.id() << " Received from: " << tt[0] << " from dir: " << dir << std::endl; 
            }


        }
    }
    return hpx::finalize();

}

int main(){

    using namespace hpx::program_options;
    return hpx::init();

}