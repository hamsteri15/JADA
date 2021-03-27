#pragma once

#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/flatten_index.hpp"

#include "slices.hpp"

using namespace JADA;



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

        return halo_begin(m_dimension, dir, m_padding);
        
    }

    position<N> barrier_end(position<N> dir) const {
        return halo_end(m_dimension, dir, m_padding);
    }


    position<N> interior_begin(position<N> dir) const {

        return boundary_begin(m_dimension, dir, m_padding);
    }



    position<N> interior_end(position<N> dir) const {

        return boundary_end(m_dimension, dir, m_padding);

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

};

template<size_t N, class T, ConnectivityType CT>
struct BoundaryData{

    BoundaryData(dimension<N> interior_dims, dimension<N> padding) : 
    m_data(create(interior_dims, padding)){}


    Partition<N, T> get(position<N> dir) const{
        auto idx = m_neighbours.idx(dir);
        return m_data.at(size_t(idx));
    } 

    
    void put(Partition<N, T> p_data, position<N> dir) const {
        auto idx = m_neighbours.idx(dir);
        m_data.at(size_t(idx)) = p_data;
    }
    


private:


    std::vector<Partition<N, T>> m_data;
    static constexpr BlockNeighbours<N, ConnectivityType::Star> m_neighbours{};







    static std::vector<Partition<N, T>> create_data(dimension<N> dims, dimension<N> padding) {

        std::vector<Partition<N, T>> ret;

        for (auto dir : m_neighbours.get()){

            dimension<N> boundary_dims{};
            for (size_t i = 0; i < N; ++i){
                if (dir[i] == 0) {
                    boundary_dims[i] = dims[i];
                }
                else {
                    boundary_dims[i] = padding[i];
                }
            }

            ret.push_back(Partition<N, T>(dims, dimension<N>{}) );

        }

        return ret;        

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
