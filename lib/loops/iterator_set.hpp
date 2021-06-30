#pragma once

#include <array>

#include "loops/index_type.hpp"
#include <boost/iterator/iterator_facade.hpp>

namespace JADA {

template <typename Iter, class S>
struct iterator_set_base
    : boost::iterator_facade<iterator_set_base<Iter, S>,
                             typename std::iterator_traits<Iter>::value_type,
                             std::random_access_iterator_tag> {

    using value_type = typename std::iterator_traits<Iter>::value_type;
    using base_type  = boost::iterator_facade<iterator_set_base<Iter, S>,
                                             value_type,
                                             std::random_access_iterator_tag>;

    // CRTP injection of access to the data, these must be implemented by all
    // derived classes
    constexpr auto& get_storage() const {
        return static_cast<const S*>(this)->get_storage();
    }
    constexpr auto& get_storage() {
        return static_cast<S*>(this)->get_storage();
    }

    constexpr auto size() const { return this->get_storage().size(); }

    bool equal(const iterator_set_base& other) const {
        return std::equal(this->get_storage().begin(),
                          this->get_storage().end(),
                          other.get_storage().begin());
    }

    // This should probably return some storage of elements
    // typename base_type::reference dereference() const { return
    // *(this->get_center()); }

    constexpr void increment() {
        for (auto& i : this->get_storage()) { ++i; }
    }

    constexpr void decrement() {
        for (auto& i : this->get_storage()) { --i; }
    }

    constexpr void advance(std::ptrdiff_t n) {
        for (auto& i : this->get_storage()) { i += n; }
    }

    constexpr Iter at(size_t i) { return this->get_storage().at(i); }
};








template <typename Iter>
struct dynamic_iterator_set
    : public iterator_set_base<Iter, dynamic_iterator_set<Iter>> {

    using storage_t = std::vector<Iter>;

    constexpr dynamic_iterator_set() = default;
    constexpr dynamic_iterator_set(storage_t iters)
        : m_storage(iters) {}
    constexpr dynamic_iterator_set(std::initializer_list<Iter> iters)
        : m_storage(iters) {}

    constexpr auto& get_storage() const { return m_storage; }
    constexpr auto& get_storage() { return m_storage; }

private:
    storage_t m_storage;
};






template <size_t L, typename Iter>
struct static_iterator_set
    : public iterator_set_base<Iter, static_iterator_set<L, Iter>> {

    using storage_t = std::array<Iter, L>;

    constexpr static_iterator_set() = default;
    constexpr static_iterator_set(storage_t storage)
        : m_storage(storage) {}
    constexpr static_iterator_set(std::initializer_list<Iter> list) {
        if (list.size() > L) {
            throw std::logic_error(
                "Invalid paramenter count for static_iterator_set");
        }
        std::move(list.begin(), list.end(), m_storage.begin());
    }

    constexpr auto& get_storage() const { return m_storage; }
    constexpr auto& get_storage() { return m_storage; }

private:
    storage_t m_storage;
};

} // namespace JADA