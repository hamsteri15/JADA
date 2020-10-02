#pragma once

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



}