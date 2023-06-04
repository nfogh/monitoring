#pragma once

#include "minmax.hpp"

namespace Monitoring {

template<typename T, typename Getter> auto Between(T min, T max, Getter getter)
{
  return [minCheck = Min(std::move(min), getter), maxCheck = Max(std::move(max), getter)](
           const T &val) { return minCheck(val) && maxCheck(val); };
}

template<typename T> auto Between(T min, T max)
{
  return Between(std::move(min), std::move(max), Intern::Identity<T>());
}

}// namespace Monitoring
