#pragma once

#include "intern.hpp"
#include "logic_callable.hpp"
#include <algorithm>
#include <utility>
#include <vector>

namespace Monitoring {

template<typename Getter, typename T> auto Equals(Getter getter, std::initializer_list<T> otherVals)
{
  auto lamb = [otherVals = std::vector<T>(otherVals), getter = std::move(getter)](const auto &, const T &val) {
    return std::any_of(otherVals.cbegin(), otherVals.cend(), [&val, getter = std::move(getter)](const auto &otherVal) {
      return getter(val) == getter(otherVal);
    });
  };
  return LogicCallable(std::move(lamb));
}

template<typename T> inline auto Equals(std::initializer_list<T> vals) { return Equals(Intern::Identity<T>(), vals); }
template<typename T> inline auto Equals(T val) { return Equals(Intern::Identity<T>(), { std::move(val) }); }

}// namespace Monitoring
