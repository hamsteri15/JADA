

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




using namespace JADA; // :(

template<size_t N, class T>
struct Partition : public Block<N>{


    Partition() = default;

    Partition(position<N> begin, position<N> extent) : 
    Block<N>(begin, extent), m_data(this->size()) {}


    explicit Partition(dimension<N> dims) : 
    Partition(position<N>{}, position<N>(dims)) {}




private:

    std::vector<T> m_data;

};




int hpx_main() {


    size_t n_partitions = 3;

    size_t global_nx = 11;
    size_t global_ny = 12;


    Decomposition<2> dec({global_ny, global_nx}, n_partitions, {false, false});


    using local_data_t = Partition<2, double>;
    using global_data_t = std::vector<local_data_t>;


    global_data_t global_data(n_partitions);

    for (size_t i = 0; i < n_partitions; ++i) {

        idx_t id = idx_t(i);
        
        //auto [local_ny, local_nx] = dec.local_grid_dimensions(id); 

        global_data[i] = local_data_t(dec.local_grid_dimensions(id));



        //global_data[i] = local_data_t(local_nx * local_ny);



    }


    for (auto g : global_data) {
        std::cout << g.size() << std::endl;
    }

 


    /*

    std::size_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    std::size_t num_worker_threads = hpx::get_num_worker_threads();
    std::size_t rank = hpx::get_locality_id();





    HpxMdCommunicator<double> comm(
        rank, 
    );


    */


    return hpx::finalize();
}

//int main(int argc, char *argv[]) {
int main() {
  using namespace hpx::program_options;


  return hpx::init();
}

