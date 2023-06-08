#pragma once

#include "intern.hpp"
#include "logic_callable.hpp"
#include <algorithm>
#include <utility>

namespace Monitoring {

template<typename Getter, typename T> auto Equals(Getter getter, T otherVal)
{
  auto lamb = [otherVal = std::move(otherVal), getter = std::move(getter)](
                const T &val) { return getter(val) == getter(otherVal); };
  return LogicCallable<decltype(lamb)>(std::move(lamb));
}

template<typename Getter, typename T> auto Equals(Getter getter, std::vector<T> otherVals)
{
  auto lamb = [otherVals = std::move(otherVals), getter = std::move(getter)](const T &val) {
    return std::any_of(otherVals.cbegin(), otherVals.cend(), [&val, getter = std::move(getter)](const auto &otherVal) {
      getter(val) == getter(otherVal);
    });
  };
  return LogicCallable<decltype(lamb)>(std::move(lamb));
}

template<typename T> inline auto Equals(std::vector<T> vals) { return Equals(Intern::Identity<T>(), std::move(vals)); }
template<typename T> inline auto Equals(T val) { return Equals(Intern::Identity<T>(), val); }

}// namespace Monitoring
