#pragma once

#include "logic_callable.hpp"
#include <cstddef>
#include <tuple>

namespace Monitoring {
namespace Intern {
  template<typename Fn, typename... Args, std::enable_if_t<std::is_member_pointer<std::decay_t<Fn>>{}, int> = 0>
  constexpr decltype(auto) invoke(Fn &&f, Args &&...args) noexcept(
    noexcept(std::mem_fn(f)(std::forward<Args>(args)...)))
  {
    return std::mem_fn(f)(std::forward<Args>(args)...);
  }

  template<typename Fn, typename... Args, std::enable_if_t<!std::is_member_pointer<std::decay_t<Fn>>{}, int> = 0>
  constexpr decltype(auto) invoke(Fn &&f, Args &&...args) noexcept(
    noexcept(std::forward<Fn>(f)(std::forward<Args>(args)...)))
  {
    return std::forward<Fn>(f)(std::forward<Args>(args)...);
  }

  template<class F, class Tuple, std::size_t... I>
  constexpr decltype(auto) apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>)
  {
    return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
  }

  template<class F, class Tuple> constexpr decltype(auto) apply(F &&f, Tuple &&t)
  {
    return apply_impl(std::forward<F>(f),
      std::forward<Tuple>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
  }
}// namespace Intern

//@brief Pick out a number of arguments from the supplied arguments
template<size_t... ArgIndices, typename InnerConditionT> auto Args(InnerConditionT innerCondition)
{
  auto lamb = [innerCondition = std::move(innerCondition)](auto &&opt, auto &&...args) {
    const auto argsTuple = std::forward_as_tuple(args...);
    return Intern::apply(innerCondition,
      std::tuple_cat(std::make_tuple(opt), std::forward_as_tuple(std::get<ArgIndices>(std::move(argsTuple))...)));
  };
  return LogicCallable<decltype(lamb)>(std::move(lamb));
}

//@brief Picks out the first argument
template<typename Func> auto FirstArg(Func func) { return Args<0>(std::move(func)); };

//@brief Picks out the second argument
template<typename Func> auto SecondArg(Func func) { return Args<1>(std::move(func)); };

//@brief Picks out the third argument
template<typename Func> auto ThirdArg(Func func) { return Args<2>(std::move(func)); };

//@brief Picks out the fourth argument
template<typename Func> auto FourthArg(Func func) { return Args<3>(std::move(func)); };
}// namespace Monitoring