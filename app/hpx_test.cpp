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

#include "communicator.hpp"

using namespace JADA;


//using communication_type = MdArray<2, double>; //this would require a serialization method
using communication_type = std::vector<double>;
HPX_REGISTER_CHANNEL_DECLARATION(communication_type)
HPX_REGISTER_CHANNEL(communication_type, stencil_communication)
using communicator_type = communicator<communication_type>;

using own_communicator_type = communicator_own<communication_type>;


template<class T>
void print(std::vector<T> v) {

    std::cout << "{ ";
    for (auto vv : v) {
        std::cout << vv << std::endl;
    }
    std::cout << "}";


}


int hpx_main(){


    size_t Nx = 12;
    size_t Ny = 13;


    std::size_t rank = hpx::get_locality_id();
    std::size_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::size_t num_local_partitions = 2;
    std::size_t num_partitions = num_localities * num_local_partitions;





    Decomposition<2> dec(
        {Ny, Nx},
        num_partitions,
        {true, true}
    );


    idx_t id = 0; //this is the id of the partition


    std::vector<own_communicator_type> comms;
    std::vector<communicator_type> comms2;

    for (idx_t i = 0; i < idx_t(num_local_partitions); ++i){

        comms.push_back(
            own_communicator_type(i, dec)
        );


        comms2.push_back(
            communicator_type(size_t(i), num_local_partitions)
        );



    }    


    /*
    comms2[0].set(communicator_type::neighbor::down, {1.0, 1.0, 1.0}, 0);
    auto t = comms2[1].get(communicator_type::neighbor::up, 0);
    print(t.get());
    */
    

    comms[0].set({1,0}, {1.0, 2.0, 3.0}, 0);
    auto tt = comms[1].get({1, 0}, 0);
    print(tt.get());
    

    



    return hpx::finalize();

}

int main(){

    using namespace hpx::program_options;
    return hpx::init();

}