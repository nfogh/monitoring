#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace Monitoring {
namespace intern {
  template<typename F, typename... Args>
  struct is_invocable
    : std::is_constructible<std::function<void(Args...)>,
        std::reference_wrapper<typename std::remove_reference<F>::type>>
  {
  };

  template<typename CallableT, typename... Args> using is_const_invocable = is_invocable<const CallableT, Args...>;

  template<typename CallableT> struct NotCallable
  {
    explicit NotCallable(CallableT callable) : mCallable(std::move(callable)) {}

    template<typename... Args, std::enable_if_t<!intern::is_const_invocable<CallableT, Args...>::value, int> = 0>
    auto operator()(Args &&...args)
    {
      return !mCallable(std::forward<Args>(args)...);
    }

    template<typename... Args, std::enable_if_t<intern::is_const_invocable<CallableT, Args...>::value, int> = 0>
    auto operator()(Args &&...args) const
    {
      return !mCallable(std::forward<Args>(args)...);
    }

  private:
    CallableT mCallable;
  };

  template<typename Lhs, typename Rhs, typename Op> struct MergedCallables
  {
    explicit MergedCallables(Lhs &&lhs, Rhs &&rhs, Op &&op)
      : mLhs(std::forward<Lhs>(lhs)), mRhs(std::forward<Rhs>(rhs)), mOp(std::forward<Op>(op))
    {}

    template<typename... Args,
      std::enable_if_t<!intern::is_const_invocable<Lhs, Args...>::value
                         || !intern::is_const_invocable<Rhs, Args...>::value,
        int> = 0>
    auto operator()(Args &&...args)
    {
      const auto res1 = mLhs(std::forward<decltype(args)>(args)...);
      const auto res2 = mRhs(std::forward<decltype(args)>(args)...);
      return mOp(res1, res2);
    }

    template<typename... Args,
      std::enable_if_t<intern::is_const_invocable<Lhs, Args...>::value
                         && intern::is_const_invocable<Rhs, Args...>::value,
        int> = 0>
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

  template<typename Lhs, typename Rhs, typename Op> auto MergeCallables(Lhs &&lhs, Rhs &&rhs, Op &&op)
  {
    return MergedCallables<Lhs, Rhs, Op>(std::forward<Lhs>(lhs), std::forward<Rhs>(rhs), std::forward<Op>(op));
  }
}// namespace intern
// Wrapping your callables in this class makes it possible to do logical operations on them.
// The callables must return a bool and take the same parameters.
// In fact, only the first callable needs to be wrapped in this class, the others can be normal callables.
// TODO: static_assert if CallableT is not an actual callable.
// TODO: Forward noexcept.
template<typename CallableT> struct LogicCallableT
{
  LogicCallableT(CallableT callable) : mCallable(std::move(callable)) {}

  template<typename... Args, std::enable_if_t<intern::is_const_invocable<CallableT, Args...>::value, int> = 0>
  auto operator()(Args &&...args) const
  {
    return mCallable(std::forward<Args>(args)...);
  }

  template<typename... Args, std::enable_if_t<!intern::is_const_invocable<CallableT, Args...>::value, int> = 0>
  auto operator()(Args &&...args)
  {
    return mCallable(std::forward<Args>(args)...);
  }

  template<typename OtherCallableT> auto operator||(OtherCallableT otherCallable)
  {
    auto lamb = intern::MergeCallables(mCallable, std::move(otherCallable), std::logical_or<bool>());
    return LogicCallableT<decltype(lamb)>(lamb);
  }

  template<typename OtherCallableT> auto operator&&(OtherCallableT otherCallable)
  {
    auto lamb = intern::MergeCallables(mCallable, std::move(otherCallable), std::logical_and<bool>());
    return LogicCallableT<decltype(lamb)>(lamb);
  }

  auto operator!()
  {
    auto lamb = intern::NotCallable<CallableT>(mCallable);
    return LogicCallableT<decltype(lamb)>(lamb);
  }

private:
  CallableT mCallable;
};

template<typename CallableT> auto LogicCallable(CallableT &&callable) { return LogicCallableT<CallableT>(callable); }
}// namespace Monitoring
