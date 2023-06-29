#pragma once

#include <Monitoring/conditions/minmax.hpp>

namespace Monitoring {

template<typename T>
struct SubscribingMaxComparison
{
    SubscribingMaxComparison(T max) : mMax(std::move(max)) {}

    template<typename SubscriberFuncT, typename T>
    bool operator()(SubscriberFuncT func, const T& arg) const
    {
        if (!subscribed) {
            func(mMax);
            subscribed = true;
        }

        return mMax(std::forward<Args>(args)...);
    }
    private:
    bool subscribed = false;
    T mMax;
}

template<
  typename Getter
> auto Max(Getter getter, FloatMessage max)
{
  return Comparison(std::move(getter), std::move(max), std::greater_equal<FloatMessage>());
}

}// namespace Monitoring
