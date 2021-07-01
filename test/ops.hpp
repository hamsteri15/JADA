#pragma once

#include "loops/position.hpp"
#include "loops/position_set.hpp"

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


struct TestOp{

    static constexpr position<2> center = {0,0};

    static constexpr std::array<position<2>, 4> stencil = {
        position<2>{0, -1},
        position<2>{0, 1},
        position<2>{1, 0},
        position<2>{-1, 0}};

    template <class Some>
    auto operator()(Some s) const {

        return *s.at(0) + *s.at(1) + *s.at(2) + *s.at(3);
        /*
        return in.at(pos + position<2>{0, -1}) +
               in.at(pos + position<2>{0, 1}) + 
               in.at(pos + position<2>{1, 0}) +
               in.at(pos + position<2>{-1, 0});
        */
    
    }



};





struct OpStar {

    static constexpr ConnectivityType CT = ConnectivityType::Star;

    static constexpr std::array<position<2>, 4> neighbours = {
        position<2>{0, -1},
        position<2>{0, 1},
        position<2>{1, 0},
        position<2>{-1, 0}};

    template <class Some>
    auto operator()(position<2> pos, const Some& in) const {

        return in.at(pos + position<2>{0, -1}) +
               in.at(pos + position<2>{0, 1}) + 
               in.at(pos + position<2>{1, 0}) +
               in.at(pos + position<2>{-1, 0});
    }

    static auto get_offsets() {

        return neighbours;



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