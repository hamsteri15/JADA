#pragma once

namespace JADA::Utils {

// Macro to handle noexcept and runtime_assert

#ifdef DEBUG
constexpr bool can_throw = false;
#else
constexpr bool can_throw = true;
#endif

} // namespace Utils
