#include <monitoring/monitoring.hpp>

#include "PubSub/PubSub.hpp"
#include "PubSubMonitor.hpp"
#include "IntMessage.hpp"
#include "FloatMessage.hpp"

auto FloatField()
{
    return [](const PubSubMonitoring::FloatMessage& msg){ return msg.GetF(); };
}

auto IntField()
{
    return [](const PubSubMonitoring::IntMessage& msg){ return msg.GetI(); };
}

int main()
{
    using namespace Monitoring;
    using namespace PubSubMonitoring;
    FloatMessage floatMessage1;
    PubSub ps;

    auto monitor = PubSubMonitoring::Monitor(floatMessage1)
      .Require(Min(FloatField(), 1.0f) && Max(FloatField(), 10.0f))
      .Handler([](bool good){ std::cout << std::boolalpha << "Good? " << good << std::endl; }).With(ps);

    for (auto i = 0.0f; i <= 15.0f; i = i + 1.0f)
    {
        std::cout << i << ": ";
        ps.Publish(FloatMessage(i));
    }

    IntMessage intMessage1(0, "IntMessage1");
    IntMessage intMessage2(0, "IntMessage2");
    auto monitor2 = PubSubMonitoring::Monitor(intMessage1, intMessage2)
      .Require(MaxDifference(IntField(), 10))
      .Handler([](bool good){ std::cout << std::boolalpha << "Good? " << good << std::endl; }).With(ps);

    for (auto i = 0; i <= 15; ++i)
    {
        std::cout << i << ": ";
        ps.Publish(IntMessage(i, "IntMessage1"));
        ps.Publish(IntMessage(i, "IntMessage2"));
    }
}
