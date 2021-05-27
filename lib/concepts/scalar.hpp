#pragma once

#include <concepts>

namespace JADA{

template<typename T>
concept Scalar = std::is_arithmetic_v<T>;

}