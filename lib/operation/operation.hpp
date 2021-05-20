#pragma once

#include <tuple>
#include "loops/position.hpp"

namespace JADA{

namespace detail {
template <class Operand, class Position> auto subscript(Operand const& v, Position pos) {

    if constexpr (std::is_arithmetic_v<Operand>) {
        return v;
    } else {
        return v[pos];
    }
}

} // namespace detail

template<class callable, class... operands>
class Operation{

public:

    
    Operation(callable f, operands const&... args) : m_f(f), m_args(args...) {}
    
    template<size_t N>
    auto operator[](position<N> pos) const{

        auto const call_at_index = [this, pos] (operands const&... a){
            return m_f(detail::subscript(a, pos)...);
        };
    
        return std::apply(call_at_index, m_args);
    }


    
    constexpr auto dims() const {
        
        //For unary expressions the LHS is the only parameter, 
        //which cant be a scalar value
        using LHS = std::tuple_element_t<0, std::tuple<operands...>>;
        constexpr bool lhs_scalar = std::is_arithmetic_v<LHS>;

        if constexpr (!lhs_scalar) {
            return std::get<0>(m_args).local_dimension();
        }

        //This is called if and only if lhs_is a scalar and RHS is either expression or array
        else {
            return std::get<1>(m_args).local_dimension();
        }
       
    }
    
    constexpr auto decomposition() const {
        
        //For unary expressions the LHS is the only parameter, 
        //which cant be a scalar value
        using LHS = std::tuple_element_t<0, std::tuple<operands...>>;
        constexpr bool lhs_scalar = std::is_arithmetic_v<LHS>;

        if constexpr (!lhs_scalar) {
            return std::get<0>(m_args).decomposition();
        }

        //This is called if and only if lhs_is a scalar and RHS is either expression or array
        else {
            return std::get<1>(m_args).decomposition();
        }
       
    }

private:
    callable m_f;
    std::tuple<operands const&...> m_args;
    //size_t m_size;
};

}