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


//TODO: move to lib/
template<class Scheme>
struct mirrorBc{

    using stencil_t = Scheme::stencil_op_t;




    static auto left_stencil = mirror_front(stencil_t{}, stencil_t::left_haldwidth());
    static auto right_stencil = mirror_back(stencil_t{}, stencil_t::right_halfwidth());



};


struct stepper{


    using scheme = FdmDerivative<1, Direction::i, DDcd4>;

    mirrorBc<scheme> mirror;

    stepper(const UniformGrid<1>& grid) : m_grid(grid), dd_x(grid) 
    {}


    std::vector<double> next(const std::vector<double>& current){

        //do left boundary stuff

        auto R = dd_x(current);
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

int main(){

    UniformGrid<1> grid({params::ni}, {params::Lx});
    stepper step(grid);

    std::vector<double> current(params::ni);

    for (idx_t i = 0; i < current.size(); ++i){
        current[i] = double(i*i);
    }


    print(current);

    double time = 0;
    while (time < params::end_t){

        current = step.next(current);
        break;
        
        
        time+=params::dt;

    }
    std::cout << "done" << std::endl;
    print(current);
    return 0;
}











