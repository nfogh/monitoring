#pragma once

#include <type_traits>
#include <utility>

namespace Monitoring {
namespace intern {
  template<typename CallableT, typename... Args>
  inline constexpr bool is_const_invocable_v = std::is_invocable_v<const CallableT, Args...>;

  template<typename CallableT> struct NotCallable
  {
    explicit NotCallable(CallableT callable) : mCallable(std::move(callable)) {}

    template<typename... Args, std::enable_if_t<!intern::is_const_invocable_v<CallableT, Args...>, int> = 0>
    auto operator()(Args &&...args)
    {
      return !mCallable(std::forward<Args>(args)...);
    }

    template<typename... Args, std::enable_if_t<intern::is_const_invocable_v<CallableT, Args...>, int> = 0>
    auto operator()(Args &&...args) const
    {
      return !mCallable(std::forward<Args>(args)...);
    }

  private:
    CallableT mCallable;
  };

  template<typename Lhs, typename Rhs, typename Op> struct MergedCallables
  {
    explicit MergedCallables(Lhs lhs, Rhs rhs, Op op) : mLhs(std::move(lhs)), mRhs(std::move(rhs)), mOp(op) {}

    template<typename... Args,
      std::enable_if_t<!intern::is_const_invocable_v<Lhs, Args...> || !intern::is_const_invocable_v<Rhs, Args...>,
        int> = 0>
    auto operator()(Args &&...args)
    {
      const auto res1 = mLhs(std::forward<decltype(args)>(args)...);
      const auto res2 = mRhs(std::forward<decltype(args)>(args)...);
      return mOp(res1, res2);
    }

    template<typename... Args,
      std::enable_if_t<intern::is_const_invocable_v<Lhs, Args...> && intern::is_const_invocable_v<Rhs, Args...>, int> =
        0>
    auto operator()(Args &&...args) const
    {
      const auto res1 = mLhs(std::forward<decltype(args)>(args)...);
      const auto res2 = mRhs(std::forward<decltype(args)>(args)...);
      return mOp(res1, res2);
    }

  private:
    Lhs mLhs;
    Rhs mRhs;
    Op mOp;
  };
}// namespace intern
// Wrapping your callables in this class makes it possible to do logical operations on them.
// The callables must return a bool and take the same parameters.
// In fact, only the first callable needs to be wrapped in this class, the others can be normal callables.
// TODO: static_assert if CallableT is not an actual callable.
// TODO: Forward noexcept.
template<typename CallableT> struct LogicCallable
{
  LogicCallable(CallableT callable) : mCallable(std::move(callable)) {}

  template<typename... Args, std::enable_if_t<intern::is_const_invocable_v<CallableT, Args...>, int> = 0>
  auto operator()(Args &&...args) const
  {
    return mCallable(std::forward<Args>(args)...);
  }

  template<typename... Args, std::enable_if_t<!intern::is_const_invocable_v<CallableT, Args...>, int> = 0>
  auto operator()(Args &&...args)
  {
    return mCallable(std::forward<Args>(args)...);
  }

  template<typename OtherCallableT> auto operator||(OtherCallableT otherCallable)
  {
    auto lamb = intern::MergedCallables(mCallable, std::move(otherCallable), std::logical_or());
    return LogicCallable<decltype(lamb)>(lamb);
  }

  template<typename OtherCallableT> auto operator&&(OtherCallableT otherCallable)
  {
    auto lamb = intern::MergedCallables(mCallable, std::move(otherCallable), std::logical_and());
    return LogicCallable<decltype(lamb)>(lamb);
  }

  auto operator!()
  {
    auto lamb = intern::NotCallable(mCallable);
    return LogicCallable<decltype(lamb)>(lamb);
  }

private:
  CallableT mCallable;
};
}// namespace Monitoring
