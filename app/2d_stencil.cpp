

#include <hpx/algorithm.hpp>
#include <hpx/chrono.hpp>
#include <hpx/init.hpp>
#include <hpx/program_options.hpp>

#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/include/partitioned_vector_predef.hpp>
#include <hpx/include/partitioned_vector_view.hpp>
#include <hpx/components/containers/coarray/coarray.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <numeric>
#include <array>


#include "grid/decomposition.hpp"
#include "grid/block.hpp"
#include "communication/hpx_md_communicator.hpp"

#include "stencil.hpp"
#include "partition.hpp"
#include "comm.hpp"
#include "solve.hpp"
#include "print.hpp"




using local_data_t = Partition<2, double>;
using global_data_t = std::vector<local_data_t>;






void put_to_barrier(local_data_t& local_data, const local_data_t& recv_data, position<2> dir) {
    //note assumes that the recv_data size is already sliced

    position<2> f_begin{};
    position<2> f_end(recv_data.dimensions());

    position<2> t_begin = local_data.barrier_begin(dir);
    position<2> t_end = local_data.barrier_end(dir);

    put_slice(recv_data, local_data, f_begin, f_end, t_begin, t_end);

}







void fill_barrier(const global_data_t& global_data, local_data_t& local_data, Comm comm, position<2> dir) {

    auto neighbour_id = comm.neigbour_id(dir);

    //get data from neighbour and put to my barriers
    if (neighbour_id != NEIGHBOUR_ID_NULL) {
    
        auto neighbour_data = comm.get_neighbour_data(global_data, dir);

        auto recv = get_slice(neighbour_data, neighbour_data.interior_begin(-dir), neighbour_data.interior_end(-dir));                
        put_to_barrier(local_data, recv, dir);
        
    }
    //fill barriers with ones
    else {
        
        for (auto pos : md_indices(local_data.barrier_begin(dir), local_data.barrier_end(dir))){
            local_data[pos] = 1.0; 
        }

    } 

}

local_data_t solve_boundary(const local_data_t& in, position<2> dir) {

    
    return solve(in, in.interior_begin(dir), in.interior_end(dir));


}




void do_work(const MockComm& comm, global_data_t& global_data) {


    auto comms = comm.get_comms();
    Utils::runtime_assert(comms.size() == global_data.size(), "Size mismatch in partition and comms" );

    
    //make a copy
    global_data_t temp = global_data;



    for (size_t i = 0; i < comms.size(); ++i) {

        auto& local_in  = global_data.at(i);
        auto&       local_out = temp.at(i);

        
        const auto& local_comm = comms.at(i);
        for (auto dir : stencil::neighbour_dirs) {
            fill_barrier(global_data, local_in, local_comm, dir);
            //auto b_sol = solve_boundary(local_in, dir);

            solve(local_in, local_out, local_in.interior_begin(dir), local_in.interior_end(dir));

        }
        
        
        position<2> dir = {0,0}; //interior
        solve(
            local_in, 
            local_out, 
            local_in.interior_begin(dir) + idx_t(1), 
            local_in.interior_end(dir)   - idx_t(1)
        );
        
    }

    std::swap(global_data, temp);



}



global_data_t init(Decomposition<2> dec){


    auto n = dec.dimensions().elementwise_product();

    global_data_t global_data(n);

    for (size_t i = 0; i < n; ++i) {

        idx_t id = idx_t(i);

        auto local_dims = dec.local_grid_dimensions(id); 
        
        auto local_data = local_data_t(local_dims, stencil::padding);

        for (auto pos : md_indices(local_data.interior_begin({0,0}), local_data.interior_end({0,0}) )){

            //local_data[pos] = double(id + 1);
            local_data[pos] = double(1);

        }

        global_data[i] = local_data;
    }


    return global_data;


}






int hpx_main() {


    size_t n_partitions = 3;

    size_t global_nx = 11;
    size_t global_ny = 12;


    Decomposition<2> dec({global_ny, global_nx}, n_partitions, {false, false});


    auto global_data = init(dec);

    MockComm comm(dec);


    //print(global_data);

    do_work(comm, global_data);

    print(global_data);



    return hpx::finalize();
}

//int main(int argc, char *argv[]) {
int main() {
  using namespace hpx::program_options;


  return hpx::init();
}

