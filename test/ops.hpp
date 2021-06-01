#pragma once

#include "loops/position.hpp"


namespace JADA{

struct OneDLinear {


    static constexpr ConnectivityType CT = ConnectivityType::Star;


    template <class Some>
    auto operator()(position<1> pos, const Some& in) const {


        return in.at(pos + position<1>{-1}) +
               -2 * in.at(pos + position<1>{0}) + 
               in.at(pos + position<1>{1});
        
    }
};





struct OpStar {

    static constexpr ConnectivityType CT = ConnectivityType::Star;
    
    template <class Some>
    auto operator()(position<2> pos, const Some& in) const {

        return in.at(pos + position<2>{0, -1}) +
               in.at(pos + position<2>{0, 1}) + 
               in.at(pos + position<2>{1, 0}) +
               in.at(pos + position<2>{-1, 0});
    }
};

struct OpBox {

    static constexpr ConnectivityType CT = ConnectivityType::Star;
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