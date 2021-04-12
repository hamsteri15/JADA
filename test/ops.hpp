#pragma once

#include "loops/position.hpp"


namespace JADA{

struct OpStar {

    template <class Some>
    auto operator()(position<2> pos, const Some& in) const {

        return in.at(pos + position<2>{0, -1}) +
               in.at(pos + position<2>{0, 1}) + 
               in.at(pos + position<2>{1, 0}) +
               in.at(pos + position<2>{-1, 0});
    }
};

struct OpBox {

    template <class Some>
    auto operator()(position<2> pos, const Some& in) const {

        return in.at(pos + position<2>{0, -1}) +
               in.at(pos + position<2>{0, 1}) + 
               in.at(pos + position<2>{1, 0}) +
               in.at(pos + position<2>{-1, 0}) +
               in.at(pos + position<2>{1, 1}) + 
               in.at(pos + position<2>{-1, 1}) + 
               in.at(pos + position<2>{-1, -1}) + 
               in.at(pos + position<2>{1, -1}); 
               
    }
};

}