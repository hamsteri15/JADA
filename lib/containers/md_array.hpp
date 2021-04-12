#pragma once
#include <vector>
#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"

namespace JADA{

template<size_t N, class T, StorageOrder SO = StorageOrder::RowMajor>
struct MdArray{

    using storage_t = std::vector<T>;

    MdArray() = default;


    MdArray(dimension<N> dim) : m_dim(dim), m_data(dim.elementwise_product()) 
    {}

    MdArray(const storage_t& data, dimension<N> dim)
        : m_dim(dim)
        , m_data(data){}

    size_t size() const {
        return m_data.size();
    }


    const storage_t& get_storage() const {return m_data;}
    storage_t& get_storage() {return m_data;}


    T* data()             {return m_data.data();}
    const T* data() const {return m_data.data();}


    T& operator[](position<N> pos) {

        return m_data[static_cast<size_t>(flatten<N, SO>(m_dim, pos))];
    }

    const T& operator[](position<N> pos) const {

        return m_data[static_cast<size_t>(flatten<N, SO>(m_dim, pos))];
    }


    void set_all(T val) {
        for (auto & v : m_data) {
            v = val;
        }
    }



    //TODO: remove
    void pretty_print() const {

        if (N < 3) {

            for (size_t j = 0; j < m_dim[0]; ++j){
            for (size_t i = 0; i < m_dim[1]; ++i){

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

    dimension<N> m_dim;
    storage_t    m_data;

};


}
