#pragma once

#include <iostream>

#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"


namespace JADA{



template<size_t N, class S>
struct MdIndexable{


    // clang-format off
    static_assert(N > 0, "MdIndexable has to have at least one spatial dimension.");

    static constexpr StorageOrder SO = StorageOrder::RowMajor;    


    //CRTP injection of access to the data and dimensions, these must be implemented by all derived classes
    constexpr const auto* get_ptr() const { return static_cast<const S*>(this)->get_ptr(); }
    constexpr       auto* get_ptr()       { return static_cast<      S*>(this)->get_ptr(); }
    constexpr dimension<N> const get_dimension() const { return static_cast<const S*>(this)->get_dimension(); }


    constexpr size_t size() const {return get_dimension().elementwise_product();}



    inline constexpr const auto& operator[](size_t idx) const { return get_ptr()[idx]; }
    inline constexpr     auto& operator[](size_t idx)       { return get_ptr()[idx]; }

    inline constexpr const auto& operator[](position<N> pos) const {
        return get_ptr()[flatten<N, SO>(this->get_dimension(), pos)];
    }

    inline constexpr auto& operator[](position<N> pos) {
        return get_ptr()[flatten<N, SO>(this->get_dimension(), pos)];
    }



    inline constexpr auto*                 data()      noexcept       {return get_ptr();}
    inline constexpr const auto*           data()      const noexcept {return get_ptr();}


    inline constexpr void set_all(auto val) {
        for (size_t i = 0; i < this->size(); ++i){
            data()[i] = val;
        }
    }


    //TODO: remove
    void pretty_print() const {

        if (N < 3) {

            for (size_t j = 0; j < get_dimension()[0]; ++j){
            for (size_t i = 0; i < get_dimension()[1]; ++i){

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




    // clang-format off


};


}