#pragma once

#include "logic_callable.hpp"
#include <tuple>

namespace Monitoring
{
    //@brief Pick out a number of arguments from the supplied arguments
    template<size_t... ArgIndices, typename InnerConditionT>
    auto Args(InnerConditionT innerCondition)
    {
        auto lamb = [innerCondition = std::move(innerCondition)](auto&&... args)
        {
            const auto argsTuple = std::forward_as_tuple(args...);
            return std::apply(innerCondition, std::forward_as_tuple(std::get<ArgIndices>(std::move(argsTuple))...));
        };
        return LogicCallable<decltype(lamb)>(std::move(lamb));
    }

    //@brief Picks out the first argument
    template<typename Func>
    auto FirstArg(Func func)
    {
      return Args<0>(std::move(func));
    };

    //@brief Picks out the second argument
    template<typename Func>
    auto SecondArg(Func func)
    {
      return Args<1>(std::move(func));
    };

    //@brief Picks out the third argument
    template<typename Func>
    auto ThirdArg(Func func)
    {
        return Args<2>(std::move(func));
    };

    //@brief Picks out the fourth argument
    template<typename Func>
    auto FourthArg(Func func)
    {
        return Args<3>(std::move(func));
    };
}