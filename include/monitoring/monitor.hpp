#pragma once
#include <memory>
#include <utility>

namespace Monitoring {
template<typename ConditionT, typename... HandlersT> class Monitor
{
public:
  Monitor(ConditionT condition, HandlersT... handlers)
    : mCondition(std::move(condition)), mHandlers(std::move(handlers)...)
  {}

  // Check the given types for the conditions, and call the
  // handlers with the
  template<typename... Ts> [[nodiscard]] auto operator()(Ts &&...ts)
  {
    static_assert(std::is_invocable_r_v<bool, decltype(mCondition), Ts...>,
      "*** WE CANNOT CHECK FOR THE CONDITIONS WITH THESE TYPES. PLEASE CHECK IF "
      "THE CONDITIONS TAKE THE PARAMES YOU HAVE GIVEN. ALSO, CONDITIONS HAVE TO"
      " RETURN A BOOLEAN VALUE ***");
    bool allOk = mCondition(std::forward<Ts>(ts)...);

    if constexpr (sizeof...(HandlersT) != 0) {
      std::apply(
        [allOk](auto &&...args) { (..., args(allOk)); }, mHandlers);// Call all handlers with allOk as a parameter
    } else {
      return allOk;
    }
  }

private:
  ConditionT mCondition;
  std::tuple<HandlersT...> mHandlers;
};

}// namespace Monitoring
