#pragma once

#include "loops/position.hpp"
#include "loops/iterator_set.hpp"

#include <vector>

namespace JADA {

template <size_t N> struct PositionSet {

    using element_t = position<N>;
    using storage_t = std::vector<element_t>;

    PositionSet() = default;

    explicit constexpr PositionSet(std::initializer_list<element_t> list)
        : m_storage(list) {}

    constexpr auto size() const { return m_storage.size(); }

    constexpr auto begin() const { return m_storage.begin(); }
    constexpr auto begin() { return m_storage.begin(); }
    constexpr auto cbegin() const { return m_storage.cbegin(); }

    constexpr auto end() const { return m_storage.end(); }
    constexpr auto end() { return m_storage.end(); }
    constexpr auto cend() const { return m_storage.cend(); }

    template <size_t NN>
    friend std::ostream& operator<<(std::ostream& os, const PositionSet<NN>& v);

private:
    storage_t m_storage;
};

template <size_t N>
std::ostream& operator<<(std::ostream& os, const PositionSet<N>& v) {

    os << "{";
    for (auto vv : v) { os << vv << " "; }
    os << "}";
    return os;
}



} // namespace JADA
