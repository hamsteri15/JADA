

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




using namespace JADA;

namespace stencil {

idx_t j = 0;
idx_t i = 0;

std::vector<position<2>> indices = {
    {j, i - 1}, {j, i + 1}, {j - 1, i}, {j + 1, i}, {j, i}

};

std::vector<position<2>> neighbour_dirs = {
    {j, i - 1}, {j, i + 1}, {j - 1, i}, {j + 1, i}
};

dimension<2> padding = {1,1};






} // namespace stencil

template<size_t N, class T>
struct Partition {


    Partition() = default;

    Partition(dimension<N> dims, dimension<N> padding) : 
    m_data((dims + padding * size_t(2)).elementwise_product()),
    m_dimension(dims),
    m_padding(padding)
    {}


    auto& operator[](position<N> p) {
        return m_data.at(size_t(flatten<N, StorageOrder::RowMajor>(m_dimension + m_padding*size_t(2), p)));        
    }

    const auto& operator[](position<N> p ) const{
        return m_data.at(size_t(flatten<N, StorageOrder::RowMajor>(m_dimension + m_padding*size_t(2), p)));        
    }


    auto dimensions() const {
        return m_dimension;
    }

    auto padding() const {
        return m_padding;
    }


    position<N> barrier_begin(position<N> dir) const{
        
        position<N> begin{};
        
        for (size_t i = 0; i < N; ++i) {

            if (dir[i] < 0) {
                begin[i] = 0;
            }
            else if (dir[i] > 0) {
                begin[i] = idx_t(m_dimension[i] + m_padding[i]);
            }
            else if (dir[i] == 0) {
                begin[i] = idx_t(m_padding[i]);
            }

        }
        return begin;
    }

    position<N> barrier_end(position<N> dir) const {
        return barrier_begin(dir) + compute_extent(dir);
    }


    position<N> interior_begin(position<N> dir) const {

        auto begin = barrier_begin(dir);
        begin -= dir * position<N>(m_padding);
        return begin;
    }



    position<N> interior_end(position<N> dir) const {

        return interior_begin(dir) + compute_extent(dir);

    }



    void pretty_print() const {

        if (N < 3) {

            for (size_t j = 0; j < m_dimension[0] + 2*m_padding[0]; ++j){
            for (size_t i = 0; i < m_dimension[1] + 2*m_padding[1]; ++i){

                std::cout << this->operator[]({idx_t(j),idx_t(i)}) << " ";

            }
                std::cout << std::endl;
            }


        std::cout << std::endl;

        }
        else {

            throw std::logic_error("Can not print more than two dimensions");
        }



    }



private:

    std::vector<T> m_data;
    dimension<N>   m_dimension;
    dimension<N>   m_padding;


    position<N> compute_extent(position<N> dir) const {

        position<N> extent{};

        for (size_t i = 0; i < N; ++i){
            if (std::abs(dir[i]) > 0) {
                extent[i] = idx_t(m_padding[i]);
            }
            else {
                extent[i] = idx_t(m_dimension[i]);
            }
        }

        return extent;

    }


};

template<size_t N>
dimension<N> to_dimension(position<N> pos) {

    dimension<N> ret;
    for (size_t i = 0; i < N; ++i) {
        Utils::runtime_assert(pos[i] >= 0, "Negative dimension");
        ret[i] = size_t(pos[i]);
    } 
    return ret;
}


template<size_t N, class T>
Partition<N, T> get_slice(const Partition<N, T>& p, position<N> begin, position<N> end) {


    auto new_dims = to_dimension(end - begin);
    dimension<N>  padding = dimension<N>{}; //no padding in slices
    Partition<N, T> slice(new_dims, padding);

    for (auto pos : md_indices(begin, end)) {
        slice[pos - begin] = p[pos];
    }

    return slice;

}





template <size_t N, class T>
void put_slice(const Partition<N, T>& from,
               [[maybe_unused]] Partition<N, T>&       to,
               position<N>         f_begin,
               position<N>         f_end,
               position<N>         t_begin,
               position<N>         t_end) {


    Utils::runtime_assert((f_end - f_begin) == (t_end - t_begin),
        "Dimension mismatch in put_slice"
    );

    auto temp = get_slice(from, f_begin, f_end);

    
    for (auto pos : md_indices(t_begin, t_end)) {
        to[pos] = temp[pos - t_begin];
    }
    
}







using local_data_t = Partition<2, double>;
using global_data_t = std::vector<local_data_t>;



struct Comm : public MdCommunicatorBase<2, ConnectivityType::Box>{


    Comm(idx_t id, Decomposition<2> dec) : 
    MdCommunicatorBase(id, dec) {}


    /*
    void swap_barriers(local_data_t& local, const global_data_t& global, position<N> dir) {





    }

    */



    local_data_t get_neighbour_data(global_data_t global, position<2> dir) const {
        auto id = neigbour_id(dir);
        return global.at(size_t(id));
    }

private:
  
};


struct MockComm{

    MockComm(Decomposition<2> dec) : 
    m_comms(create_comms(dec)) {} 
    

    auto get_local_comm(idx_t id) const{
        return m_comms.at(size_t(id));
    }

    auto get_comms() const {
        return m_comms;
    }



private:
    std::vector<Comm> m_comms;
    
    static std::vector<Comm> create_comms(Decomposition<2> dec) {

        size_t n_partitions = dec.dimensions().elementwise_product();
        std::vector<Comm> ret;
        for (idx_t id = 0; id < idx_t(n_partitions); ++id) {
            ret.push_back(
                Comm(id, dec)
            );
        }

        return ret;
    }


};


void put_to_barrier(local_data_t& local_data, const local_data_t& recv_data, position<2> dir) {
    //note assumes that the recv_data size is already sliced

    position<2> f_begin{};
    position<2> f_end(recv_data.dimensions());

    position<2> t_begin = local_data.barrier_begin(dir);
    position<2> t_end = local_data.barrier_end(dir);

    put_slice(recv_data, local_data, f_begin, f_end, t_begin, t_end);

}


local_data_t solve(local_data_t in, position<2> begin, position<2> end) {

    local_data_t out(in);

    for (auto pos : md_indices(begin, end)) {

        for (auto s : stencil::indices) {
            out[pos] += in[pos + s];
        }

    }
    return out;
}



void do_boundary_work(const global_data_t& global_data, local_data_t& local_data, Comm comm, position<2> dir) {

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

    //local_data = solve(local_data, local_data.interior_begin(dir), local_data.interior_end(dir));



}



void swap_boundaries(const MockComm& comm, global_data_t& global_data) {


    auto comms = comm.get_comms();
    Utils::runtime_assert(comms.size() == global_data.size(), "Size mismatch in partition and comms" );


    for (size_t i = 0; i < comms.size(); ++i) {

        auto& local_data = global_data.at(i);

        
        const auto& local_comm = comms.at(i);
        for (auto dir : stencil::neighbour_dirs) {
            do_boundary_work(global_data, local_data, local_comm, dir);
        }
        

        position<2> dir = {0,0}; //interior
        local_data = solve(
            local_data, local_data.interior_begin(dir) + idx_t(1), local_data.interior_end(dir) - idx_t(1)
        );


    }




}


void print(global_data_t global) {

    for (auto g :  global) {
        g.pretty_print();
    }

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


    print(global_data);

    swap_boundaries(comm, global_data);

    print(global_data);


    //print(global_data);    


    /*
    for (auto g : global_data) {

//        std::cout << g.dimensions() << std::endl;

        position<2> dir = {0,-1};
        std::cout << "Dims: " << g.dimensions()
                  << " Barrier begin: " << g.barrier_begin(dir)
                  << " Barrier end: " << g.barrier_end(dir)
                  << " Interior begin " << g.interior_begin(dir)
                  << " Interior end " << g.interior_end(dir)
                  << std::endl;
    }
    */
    /*
    auto global_data = init(dec);


    MockComm comm(dec);

    */





    //solve_all(global_data);







    return hpx::finalize();
}

//int main(int argc, char *argv[]) {
int main() {
  using namespace hpx::program_options;


  return hpx::init();
}

