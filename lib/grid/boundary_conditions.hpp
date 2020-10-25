#pragma once

#include <array>
#include <utility> //std::pair
#include <algorithm> //std::find_if
#include "loops/index_type.hpp"
#include "grid/boundary_condition.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{


template<idx_t Dim>
class BoundaryConditions{

public:

    explicit BoundaryConditions(std::array<BoundaryCondition, Dim*2> conditions) : 
    m_conditions(conditions)
    {
        //TODO: add some sanity check
    }



    BoundaryCondition get(Direction dir, BcLoc loc) const {

        auto find_bc = [&](const BoundaryCondition& bc){
            return (bc.dir == dir) && (bc.loc == loc);
        };

        auto bc = std::find_if(m_conditions.begin(), m_conditions.end(), find_bc);

        Utils::runtime_assert(bc != m_conditions.end(), "Boundary condition not found.");

        return *bc;

    }


    bool is_periodic(Direction dir) const {

        auto bc_begin = get(dir, BcLoc::begin);
        auto bc_end = get(dir, BcLoc::end);
        return (bc_begin.type == BcType::periodic) && (bc_end.type == BcType::periodic);
    }



private:

    std::array<BoundaryCondition, Dim*2> m_conditions;

};

}