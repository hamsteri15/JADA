#pragma once


#include <coroutine>
#include "loops/index_type.hpp"
#include "loops/position.hpp"

// THIS FILE CURRENTLY COMPILES ONLY WITH GCC VERSION >= 10

namespace JADA{


template <std::size_t N> struct index_generator {

    static_assert(N != 0, "N must be greater than 0.");

    struct promise_type {
        using return_type = index_generator;

        position<N> pos;

        constexpr std::suspend_always yield_value(position<N> pos_) noexcept {
            pos = pos_;
            return {};
        }

        constexpr std::suspend_always initial_suspend() const noexcept { return {}; }

        constexpr std::suspend_always final_suspend() const noexcept { return {}; }

        index_generator get_return_object() noexcept { return index_generator(this); }

        constexpr void return_void() noexcept {}

        constexpr void unhandled_exception() {}
    };

    struct iterator {
        std::coroutine_handle<promise_type> coro;

        bool                                     done;

        constexpr iterator(std::coroutine_handle<promise_type> coro_, bool done_)
            : coro(coro_)
            , done(done_) {}

        iterator& operator++() {
            coro.resume();
            done = coro.done();
            return *this;
        }

        position<N> operator*() const { return coro.promise().pos; }

        constexpr bool operator==(iterator const& rhs) const noexcept { return done == rhs.done; }
        constexpr bool operator!=(iterator const& rhs) const noexcept { return !(*this == rhs); }
    };

    iterator begin() {
        p.resume();
        return iterator(p, p.done());
    }

    constexpr iterator end() { return iterator(p, true); }

    constexpr index_generator(index_generator&& rhs) noexcept
        : p(rhs.p) {
        rhs.p = nullptr;
    }

    ~index_generator() {
        if (p) p.destroy();
    }

private:
    explicit index_generator(promise_type* p_) noexcept
        : p(std::coroutine_handle<promise_type>::from_promise(*p_)) {}

    std::coroutine_handle<promise_type> p;
};


}