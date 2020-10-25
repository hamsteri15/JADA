#pragma once

#include "grid/direction.hpp"
#include "grid/boundary_condition.hpp"


namespace JADA{

//Base boundary types
enum class BoundaryType{
    physical, processor, periodic
};

//Location of the boundary corredponding to a direction
enum class BoundaryLocation{
    begin, end
};


struct Boundary{
    BoundaryType type;
    BoundaryLocation location;
};

struct BoundaryLocationNew{

    Direction dir;
    bool begin; //true for for begin, false for end
};


struct BoundaryNew{


    BoundaryLocation loc;


};




}