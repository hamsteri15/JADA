#pragma once
#include <cstddef>
#include <array>

#include "utils/math_vector_base.hpp"

namespace JADA::Utils{

template <class T, size_t L>
struct MathVector : public MathVectorBase<T, L, MathVector<T, L>> {


    inline constexpr MathVector() = default;

    constexpr MathVector(std::initializer_list<T> list) {

        if (list.size() > L){
            throw std::logic_error("Invalid paramenter count for vector");
        }
        std::move(list.begin(), list.end(), m_storage.begin());
    }

    const T* get_ptr() const { return m_storage.data(); }
    T* get_ptr() { return m_storage.data(); }

private:
    std::array<T, L> m_storage{};
};
}