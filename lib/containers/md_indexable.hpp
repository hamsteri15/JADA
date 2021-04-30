#pragma once

#include "loops/position.hpp"
#include "loops/dimension.hpp"
#include "loops/flatten_index.hpp"


namespace JADA{



template<class T, size_t N, class S>
struct MdIndexable{


    // clang-format off
    static_assert(N > 0, "MdIndexable has to have at least one spatial dimension.");

    static constexpr StorageOrder SO = StorageOrder::RowMajor;    


    //CRTP injection of access to the data and dimensions, these must be implemented by all derived classes
    constexpr const T* get_ptr() const { return static_cast<const S*>(this)->get_ptr(); }
    constexpr       T* get_ptr()       { return static_cast<      S*>(this)->get_ptr(); }
    constexpr dimension<N> const get_dimension() const { return static_cast<const S*>(this)->get_dimension(); }

    inline constexpr const T& operator[](size_t idx) const { return get_ptr()[idx]; }
    inline constexpr     T& operator[](size_t idx)       { return get_ptr()[idx]; }

    inline constexpr const T& operator[](position<N> pos) const {
        return get_ptr()[flatten(this->get_dimension(), pos)];
    }

    inline constexpr T& operator[](position<N> pos) {
        return get_ptr()[flatten(this->get_dimension(), pos)];
    }



    inline constexpr T*                 data()      noexcept       {return get_ptr();}
    inline constexpr const T*           data()      const noexcept {return get_ptr();}







    // clang-format off


};


}