#pragma once

#include <functional>


namespace Monitoring {
namespace Intern {
  template<typename T> auto Identity()
  {
    return [](const T &val) { return val; };
  }

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
}// namespace Monitoring