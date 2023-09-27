#pragma once

#include "intern.hpp"
#include "logic_callable.hpp"

namespace Monitoring {

template<typename Getter, typename T> auto Between(Getter getter, T min, T max)
{
  auto lamb = [getter = std::move(getter), min = std::move(min), max = std::move(max)](
                const auto &, const T &val) { return getter(min) <= getter(val) && getter(val) <= getter(max); };
  return LogicCallable(std::move(lamb));
}

template<typename T> auto Between(T min, T max)
{
  return Between(Intern::Identity<T>(), std::move(min), std::move(max));
}

}// namespace Monitoring
