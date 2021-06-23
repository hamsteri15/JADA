#pragma once

#include "loops/index_type.hpp"


namespace JADA{








template <class S, class Shape>
struct IterativeStencilLoop {

    S current_state;
    S next_state;
    Shape shape; //shape of the state variable

    auto do_work([[maybe_unused]] idx_t i) {
        //does work on idx i
    } 


private:

    template<class s, class I>
    s pick([[maybe_unused]] I idx) {

        //picks indices I from current_state and returns a tuple
        return 0;
    }











};





}