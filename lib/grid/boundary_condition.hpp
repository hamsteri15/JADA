#pragma once

#include "grid/direction.hpp"

enum class BcLoc{
    begin, end
};


enum class BcType{
    periodic, custom
};

namespace JADA{

struct BoundaryCondition{

    BoundaryCondition(Direction dir_, BcLoc loc_, BcType type_) : dir(dir_), loc(loc_), type(type_) {}


    bool is_periodic() const {
        return type == BcType::periodic;
    }


    Direction dir;
    BcLoc     loc;
    BcType    type;

};




} // namespace name
