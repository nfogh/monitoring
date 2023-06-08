#pragma once

#include "intern.hpp"
#include <utility>

namespace Monitoring {

template<typename T, typename Getter> auto Equals(T otherVal, Getter getter)
{
  return [otherVal = std::move(otherVal), getter = std::move(getter)](
           const T &val) { return getter(val) == getter(otherVal); };
}

template<typename T> auto Equals(T vals)
{
  return Equals(std::move(vals), Intern::Identity<T>());
}

}// namespace Monitoring
