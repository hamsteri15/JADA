#pragma once

#include "communication/communicator.hpp"
#include "loops/position.hpp"

namespace JADA{

//N n-spatial dirs, T datatype, Derived the class that derives from this
template<class Derived> 
class MdCommunicatorBase{
public:

    Derived* derived() {
        return static_cast<Derived*>(this);
    }

    const Derived* derived() const {
        return static_cast<const Derived*>(this);
    }

    //CRTP injected functions

    //Checks if has a neighbour in direction dir
    template<size_t N>
    bool has_neighbour(position<N> dir) const{
        return derived().has_neighbour(dir);
    }


    //Send data to direction dir
    template<size_t N, class T>
    void set(position<N> dir, T&& data, size_t tag){
        derived().set(dir, data, tag);
    }


    //Get data from direction dir
    template<size_t N>
    auto get(position<N> dir, size_t tag) {
        return derived().get(dir, tag);
    }


};




}