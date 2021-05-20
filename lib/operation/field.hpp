#pragma once

#include <array>
#include <hpx/hpx.hpp>
#include <algorithm>

#include "containers/md_array.hpp"
#include "grid/decomposition.hpp"
#include "loops/dimension.hpp"
#include "loops/md_range_indices.hpp"
#include "loops/md_index_loops.hpp"
#include "operation/operation.hpp"


namespace JADA {

template <size_t N, class T> struct Field {

    using storage_t = MdArray<N, T>;

    Field() = default;

    explicit Field(dimension<N>        dims,
          std::array<bool, N> periodicity = periodicity_none())
        : m_dec(Decomposition<N>(dims, rank_count(), periodicity))
        , m_storage(m_dec.local_grid_dimensions(this->id())) {}


    const Field& operator=(T element){

        for (auto& e : m_storage.get_storage()) {
            e = element;
        }
        return *this;

    }
    
    template<class Callable, class... Operands>
    Field(const Operation<Callable, Operands...>& expr) :
    m_dec(expr.decomposition()),
    m_storage(expr.dims())
    {
        //TODO: debug assert that decomposition is the same
        Utils::runtime_assert(m_storage.get_dimension() == expr.dims(), "Dimension mismatch");

        auto begin = position<N>{};
        auto end   = position<N>{m_storage.get_dimension()};
        for (auto pos : md_indices(begin, end)){
            m_storage[pos] = expr[pos];
        }
    }
    


    const T& operator[](position<N> pos) const {
        return m_storage[pos];
    }

    T& operator[](position<N> pos) {
        return m_storage[pos];
    }


    dimension<N> local_dimension() const {
        return m_dec.local_grid_dimensions(id());
    }


    Decomposition<N> decomposition() const {
        return m_dec;
    }





private:
    Decomposition<N> m_dec;
    storage_t        m_storage; //Local storage

    static constexpr std::array<bool, N> periodicity_none() {
        std::array<bool, N> ret;
        for (auto& e : ret) { e = false; }
        return ret;
    }

    static size_t rank_count() {
        return hpx::get_num_localities(hpx::launch::sync);
    }

    static idx_t id() {
        return idx_t(hpx::get_locality_id());
    }



};

template<size_t N, class T>
inline auto operator+(const Field<N, T>& lhs, const Field<N, T>& rhs){

    Utils::runtime_assert(lhs.local_dimension() == rhs.local_dimension(), "Dimension mismatch");

    return Operation{
        [](auto l, auto r) {return l + r;},
        lhs,
        rhs
    };
}
/*
template<class LHS, class RHS, 
class = std::enable_if_t<SupportsBinaryExpression_v<LHS, RHS>>>
auto operator+(LHS const& lhs, RHS const& rhs) {

    return Expression{
        [](auto const& l, auto const& r) {return l+r;},
        lhs,
        rhs
    };

}
*/






} // namespace JADA