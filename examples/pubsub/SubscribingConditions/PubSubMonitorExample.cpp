#include <monitoring/monitoring.hpp>

#include "FloatMessage.hpp"
#include "IntMessage.hpp"
#include "PubSub/PubSub.hpp"
#include "PubSubMonitor.hpp"

auto FloatField()
{
  return [](const PubSubMonitoring::FloatMessage &msg) { return msg.GetF(); };
}

auto IntField()
{
  return [](const PubSubMonitoring::IntMessage &msg) { return msg.GetI(); };
}

int main()
{
  using namespace Monitoring;
  using namespace PubSubMonitoring;
  PubSub ps;

  std::cout << "Test min/max: " << std::endl;

  auto monitor = PubSubMonitoring::Monitor(FloatMessage(0.0f))
                   .Require(Min(FloatField(), FloatMessage("MinParam")) && Max(FloatField(), FloatMessage("MaxParam")))
                   .Handler([](bool good) { std::cout << std::boolalpha << "Good? " << good << std::endl; })
                   .With(ps);

  for (auto i = 0; i <= 15; ++i) {
    std::cout << i << ": ";
    ps.Publish(FloatMessage(static_cast<float>(i)));
  }

  std::cout << std::endl << "Test diff: " << std::endl;

  auto monitor2 = PubSubMonitoring::Monitor(IntMessage(0, "IntMessage1"), IntMessage(0, "IntMessage2"))
                    .Require(MaxDifference(IntField(), 10))
                    .Handler([](bool good) { std::cout << std::boolalpha << "Good? " << good << std::endl; })
                    .With(ps);

  for (auto i = -10; i <= 10; ++i) {
    std::cout << i << "," << -i << " = " << abs(2 * i) << ": ";
    ps.Publish(IntMessage(i, "IntMessage1"));
    ps.Publish(IntMessage(-i, "IntMessage2"));
  }
}
