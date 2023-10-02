#include <monitoring/monitoring.hpp>

#include "FloatMessage.hpp"
#include "IntMessage.hpp"
#include "PubSub/PubSub.hpp"
#include "PubSubMonitor.hpp"
#include "minmax.hpp"

auto FloatField()
{
  return [](const PubSubMonitoring::FloatMessage &msg) { return msg.GetF(); };
}

auto IntField()
{
  return [](const PubSubMonitoring::IntMessage &msg) { return msg.GetI(); };
}

auto PrintResult()
{
  return [](bool good) { std::cout << std::boolalpha << "Good? " << good << std::endl; };
}

auto Print()
{
  return [](bool good) { std::cout << std::boolalpha << "Good? " << good << std::endl; };
}

int main()
{
  using namespace Monitoring;
  using namespace PubSubMonitoring;
  PubSub ps;

  std::cout << "Test min/max: " << std::endl;
  auto monitor =
    PubSubMonitoring::Monitor(FloatMessage("MySignal")).With(ps).Require(Min(FloatMessage("MyLimit"))).Handler(Print());

  std::cout << "Setting new limit to 5.0\n";
  ps.Publish(FloatMessage(5.0f, "MyLimit"));

  for (auto i = 0; i <= 10; ++i) {
    std::cout << "Checking value of " << i << ": ";
    ps.Publish(FloatMessage(static_cast<float>(i), "MySignal"));
  }

  std::cout << "Setting new limit to 8.0\n";
  ps.Publish(FloatMessage(8.0f, "MyLimit"));

  for (auto i = 0; i <= 10; ++i) {
    std::cout << i << ": ";
    ps.Publish(FloatMessage(static_cast<float>(i), "MySignal"));
  }

  // std::cout << std::endl << "Test diff: " << std::endl;

  /*auto monitor2 = PubSubMonitoring::Monitor(IntMessage(0, "IntMessage1"), IntMessage(0, "IntMessage2"))
                    .Require(MaxDifference(IntField(), 10))
                    .Handler([](bool good) { std::cout << std::boolalpha << "Good? " << good << std::endl; })
                    .With(ps);

  for (auto i = -10; i <= 10; ++i) {
    std::cout << i << "," << -i << " = " << abs(2 * i) << ": ";
    ps.Publish(IntMessage(i, "IntMessage1"));
    ps.Publish(IntMessage(-i, "IntMessage2"));
  }*/
}
