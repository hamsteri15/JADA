#pragma once

#include <boost/iterator/iterator_facade.hpp>

namespace JADA{



template <typename Iter, size_t N>
struct neighbour_iterator
    : boost::iterator_facade<neighbour_iterator<Iter, N>,
                             typename std::iterator_traits<Iter>::value_type,
                             std::random_access_iterator_tag> {

    using value_type = typename std::iterator_traits<Iter>::value_type;
    using base_type  = boost::iterator_facade<neighbour_iterator<Iter, N>,
                                             value_type,
                                             std::random_access_iterator_tag>;

    neighbour_iterator(Iter center, std::array<Iter, N> neighbours)
        : m_center(center)
        , m_neighbours(neighbours) {}

    neighbour_iterator(Iter                 center,
                       size_t               offset,
                       std::array<idx_t, N> n_indices)
        : neighbour_iterator(center,
                             build_neighbour_iters(center, offset, n_indices)) {
    }

    bool equal(neighbour_iterator const& other) const {
        return m_center == other.m_center;
    }

    typename base_type::reference dereference() const { return *m_center; }

    void increment() {
        ++m_center;
        for (size_t i = 0; i < N; ++i) { ++(m_neighbours[i]); }
    }

    void decrement() {
        --m_center;
        for (size_t i = 0; i < N; ++i) { --(m_neighbours[i]); }
    }

    void advance(std::ptrdiff_t n) {
        m_center += n;
        for (size_t i = 0; i < N; ++i) { m_neighbours[i] += n; }
    }

    std::ptrdiff_t distance_to(neighbour_iterator const& other) const {
        return other.m_center - m_center;
    }

    constexpr Iter neighbour(size_t i) {
        return m_neighbours.at(i);
        //return m_neighbours[i];
    }

private:
    // friend class hpx::util::iterator_core_access;

    Iter                m_center;
    std::array<Iter, N> m_neighbours;

    static std::array<Iter, N> build_neighbour_iters(
        Iter center, size_t offset, std::array<idx_t, N> n_indices) {

        std::array<Iter, N> neighbours;
        for (size_t i = 0; i < N; ++i) {
            neighbours[i] = center + idx_t(offset) * n_indices[i];
        }
        return neighbours;
    }
};


}