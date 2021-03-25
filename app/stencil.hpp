#pragma once

#include "loops/position.hpp"
#include "loops/dimension.hpp"



namespace stencil {

using namespace JADA;
idx_t j = 0;
idx_t i = 0;

std::vector<position<2>> indices = {
    {j, i - 1}, {j, i + 1}, {j - 1, i}, {j + 1, i}, {j, i}

};

std::vector<position<2>> neighbour_dirs = {
    {j, i - 1}, {j, i + 1}, {j - 1, i}, {j + 1, i}
};

dimension<2> padding = {1,1};



} // namespace stencil