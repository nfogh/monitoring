#pragma once
#include <memory>
#include <utility>

namespace Monitoring {
template<typename ConditionT, typename HandlersT, typename OptT = int> class MonitorT
{
public:
  MonitorT(ConditionT condition, HandlersT handlers, OptT opt = 0)
    : mCondition(std::move(condition)), mHandlers(std::move(handlers)), mOpt(std::move(opt))
  {}

  // Check the given types for the conditions, and call the
  // handlers with the
  template<typename... Ts> /*[[nodiscard]]*/ auto operator()(Ts &&...ts)
  {
    /*static_assert(std::is_invocable_r_v<bool, decltype(mCondition), decltype(mOpt), Ts...>,
      "*** WE CANNOT CHECK FOR THE CONDITIONS WITH THESE TYPES. PLEASE CHECK IF "
      "THE CONDITIONS TAKE THE PARAMES YOU HAVE GIVEN. ALSO, CONDITIONS HAVE TO"
      " RETURN A BOOLEAN VALUE ***");*/

    // First parameter doesn't matter.
    bool allOk = mCondition(mOpt, std::forward<Ts>(ts)...);

    // if constexpr (std::tuple_size_v<HandlersT> != 0) {
    //   std::apply(
    //     [allOk](auto &&...args) { (..., args(allOk)); }, mHandlers);// Call all handlers with allOk as a parameter
    // } else {
    //   return allOk;
    // }
    mHandlers(allOk);
  }

  const OptT &GetOpt() const { return mOpt; }
  OptT &GetOpt() { return mOpt; }

private:
  ConditionT mCondition;
  HandlersT mHandlers;
  OptT mOpt;
};

template<typename ConditionT, typename HandlersT, typename OptT = int>
auto Monitor(ConditionT condition, HandlersT handlers, OptT opt = {})
{
  return MonitorT<ConditionT, HandlersT, OptT>(std::move(condition), std::move(handlers), std::move(opt));
}

}// namespace Monitoring
