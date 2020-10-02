#include <vector>
#include <iostream>
#include "grid/uniform_grid.hpp"
#include "operation/fdm_derivative.hpp"
#include "operation/fdm_operations.hpp"
#include "loops/serial_index_loops.hpp"


using namespace JADA;

struct params{

    static constexpr double dt = 0.001;
    static constexpr double end_t = 0.1;
    static constexpr double Lx = 1.0;
    static constexpr double kappa = 0.01;
    static constexpr idx_t  ni = 10;

};

/*
// Fourth order central-difference for the second derivative
struct DDcd4 : public StencilOperation<DDcd4> {

    static constexpr StencilIndices<5> indices = {-2, -1, 0, 1, 2};

    template <class T>
    static T
    apply(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5) {
        return T((-v1 + 16.0 * v2 - 30.0 * v3 + 16.0 * v4 - v5) / 12.0);
    }
};
*/




/*
template<class Scheme>
struct LeftBoundaryOp : public Scheme{

    static constexpr auto indices = 
    mirror_front(Scheme::indices, StencilOperation<Scheme>::left_halfwidth());

    using Scheme::apply;



};



struct stepper{


    using scheme = FdmDerivative<1, Direction::i, DDcd4>;

    LeftBoundaryOp<DDcd4> left_op;

//    mirrorBc<scheme> mirror;

    stepper(const UniformGrid<1>& grid) : m_grid(grid), dd_x(grid) 
    {}


    std::vector<double> next(const std::vector<double>& current){

        std::vector<double> R(current.size());

        //do left boundary stuff
//        scheme::apply_begin_bc(current ,R, left_op);

        //do interior stuff
        dd_x(current, R);
        return R;


        //do right boundary stuff
    }    

private:
    const UniformGrid<1>& m_grid;
    scheme dd_x;

};

template<class T>
void print(const std::vector<T>& v){

    for (auto vv : v){
        std::cout << vv << std::endl;
    }

}
*/

int main(){

    /*
    UniformGrid<1> grid({params::ni}, {params::Lx});
    stepper step(grid);


    //LeftBoundaryOp<DDcd4> left_op;

    double test = LeftBoundaryOp<DDcd4>::apply(3.0, 31.0, 31.0, 4.0, 5.0);

    std::cout << test << std::endl;


    std::vector<double> current(params::ni);

    for (idx_t i = 0; i < current.size(); ++i){
        current[i] = double(i*i);
    }


//    print(current);

    double time = 0;
    while (time < params::end_t){

        current = step.next(current);
        break;
        
        
        time+=params::dt;

    }
    std::cout << "done" << std::endl;
//    print(current);
*/
    return 0;
}











