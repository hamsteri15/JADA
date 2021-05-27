#pragma once

#include <concepts>

namespace JADA {

template <typename F, class... Args>
concept Callable = std::regular_invocable<F, Args...>;


}