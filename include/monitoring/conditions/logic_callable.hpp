#pragma once

#include <utility>

namespace Monitoring {
// Wrapping your callables in this class makes it possible to do logical operations on them.
// The callables must return a bool and take the same parameters.
// In fact, only the first callable needs to be wrapped in this class, the others can be normal callables.
// TODO: Make operator||, && and ! only give a mutable lambda if needed.
// TODO: static_assert if CallableT is not an actual callable.
template<typename CallableT> struct LogicCallable
{
  LogicCallable(CallableT callable) : mCallable(std::move(callable)) {}

  template<typename... Args> auto operator()(Args &&...args) const { return mCallable(std::forward<Args>(args)...); }

  template<typename... Args> auto operator()(Args &&...args) { return mCallable(std::forward<Args>(args)...); }

  template<typename OtherCallableT> auto operator||(OtherCallableT otherCallable)
  {
    auto lamb = [thisCallable = this->mCallable, otherCallable = std::move(otherCallable)](auto &&...args) mutable {
      const auto res1 = thisCallable(std::forward<decltype(args)>(args)...);
      const auto res2 = otherCallable(std::forward<decltype(args)>(args)...);
      return res1 || res2;
    };
    return LogicCallable<decltype(lamb)>(lamb);
  }

  template<typename OtherCallableT> auto operator&&(OtherCallableT otherCallable)
  {
    auto lamb = [thisCallable = this->mCallable, otherCallable = std::move(otherCallable)](auto &&...args) mutable {
      const auto res1 = thisCallable(std::forward<decltype(args)>(args)...);
      const auto res2 = otherCallable(std::forward<decltype(args)>(args)...);
      return res1 && res2;
    };
    return LogicCallable<decltype(lamb)>(lamb);
  }

  auto operator!()
  {
    auto lamb = [callable = this->mCallable](
                  auto &&...args) mutable { return !callable(std::forward<decltype(args)>(args)...); };
    return LogicCallable<decltype(lamb)>(lamb);
  }

private:
  CallableT mCallable;
};
}// namespace Monitoring
