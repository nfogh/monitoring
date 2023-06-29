#pragma once

#include "condition.hpp"
#include "intern.hpp"

namespace Monitoring {
namespace Intern {
  template<typename Getter> struct Even
  {
    Even(Getter getter) : mGetter(std::move(getter)) {}

    template<typename ValT> constexpr bool check(const ValT &val) const { return mGetter(val) % 2 == 0; }

    LIBMONITORING_DEFINE_LOGIC_OPERATORS;

  private:
    Getter mGetter;
  };

  template<typename Getter> struct Odd
  {
    Odd(Getter getter) : mGetter(std::move(getter)) {}

    template<typename ValT> constexpr bool check(const ValT &val) const { return mGetter(val) % 2 != 0; }

    LIBMONITORING_DEFINE_LOGIC_OPERATORS;

  private:
    Getter mGetter;
  };
}// namespace Intern

template<typename GetterT> auto Even(GetterT getter) { return Intern::Even(std::move(getter)); }
inline auto Even() { return Intern::Even(Intern::Identity()); }

template<typename GetterT> auto Odd(GetterT getter) { return Intern::Odd(std::move(getter)); }
inline auto Odd() { return Intern::Odd(Intern::Identity()); }

}// namespace Monitoring