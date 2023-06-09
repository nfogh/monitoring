#pragma once

namespace Monitoring
{
    // Picks out the "Index"'th argument and passes it to "func"
    template<size_t Index, typename Func>
    struct CParamIndex
    {
      CParamIndex(Func func) : mFunc(std::move(func)) {}

      template<typename... Args>
      auto operator()(Args&&... args)
      {
        return mFunc(std::get<Index>(std::forward_as_tuple(std::forward<Args>(args)...)));
      }

    private:
        Func mFunc;
    };

    // Picks out the "Index"'th argument and passes it to "func"
    template<size_t Index, typename Func>
    auto ParamIndex(Func func)
    {
      return CParamIndex<Index, Func>(std::move(func));
    }

    //@brief Picks out the first parameter
    template<typename Func>
    auto FirstParam(Func func)
    {
      return ParamIndex<0>(std::move(func));
    };

    //@brief Picks out the second parameter
    template<typename Func>
    auto SecondParam(Func func)
    {
      return ParamIndex<1>(std::move(func));
    };

    //@brief Picks out the third parameter
    template<typename Func>
    auto ThirdParam(Func func)
    {
        return ParamIndex<2>(std::move(func));
    };

    //@brief Picks out the fourth parameter
    template<typename Func>
    auto FourthParam(Func func)
    {
        return ParamIndex<2>(std::move(func));
    };
}