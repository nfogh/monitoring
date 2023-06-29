#pragma once

#include "condition.hpp"
#include "intern.hpp"
#include <algorithm>
#include <utility>
#include <vector>

namespace Monitoring {
namespace Intern {
  template<typename Getter, typename T> struct Equals
  {
    Equals(Getter getter, std::initializer_list<T> otherVals)
      : mGetter(std::move(getter)), mOtherVals(std::vector(otherVals))
    {}

    template<typename ValT> bool check(const ValT &val) const
    {
      return std::any_of(mOtherVals.cbegin(), mOtherVals.cend(), [&val, &getter = mGetter](const auto &otherVal) {
        return getter(val) == getter(otherVal);
      });
    }

    LIBMONITORING_DEFINE_LOGIC_OPERATORS;

  private:
    Getter mGetter;
    std::vector<T> mOtherVals;
  };
}// namespace Intern

template<typename GetterT, typename T> inline auto Equals(GetterT getter, std::initializer_list<T> vals)
{
  return Intern::Equals(std::move(getter), vals);
}

template<typename T> inline auto Equals(std::initializer_list<T> vals)
{
  return Intern::Equals(Intern::Identity(), vals);
}

template<typename GetterT, typename T> inline auto Equals(GetterT getter, T val)
{
  return Intern::Equals(std::move(getter), { std::move(val) });
}
template<typename T> inline auto Equals(T val) { return Intern::Equals(Intern::Identity(), { std::move(val) }); }

}// namespace Monitoring
