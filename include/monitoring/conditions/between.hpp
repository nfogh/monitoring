#pragma once

#include "condition.hpp"
#include "intern.hpp"

namespace Monitoring {
namespace Intern {
  template<typename Getter, typename T> struct Between
  {
    Between(Getter getter, T min, T max) : mGetter(std::move(getter)), mMin(std::move(min)), mMax(std::move(max)) {}

    template<typename ValT> bool check(const ValT &val) const
    {
      return mGetter(mMin) <= mGetter(val) && mGetter(val) <= mGetter(mMax);
    }

    LIBMONITORING_DEFINE_LOGIC_OPERATORS;

  private:
    Getter mGetter;
    T mMin;
    T mMax;
  };
}// namespace Intern

template<typename GetterT, typename T> constexpr auto Between(GetterT getter, T min, T max)
{
  return Intern::Between(std::move(getter), std::move(min), std::move(max));
}

template<typename T> constexpr auto Between(T min, T max)
{
  return Intern::Between(Intern::Identity(), std::move(min), std::move(max));
}

}// namespace Monitoring
