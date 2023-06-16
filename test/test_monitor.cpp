#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/conditions/argindex.hpp>
#include <monitoring/conditions/equals.hpp>
#include <monitoring/monitor.hpp>

using namespace Monitoring;

TEST_CASE("Monitor", "[monitor]")
{
  SECTION("monitor")
  {
    Monitor(Equals(1), [](const bool success) { REQUIRE(success); })(1);
    Monitor(
      Equals(1), [](const bool success) { REQUIRE(success); }, [](const bool success) { REQUIRE(success); })(1);
    Monitor(Equals(1.0), [](const bool success) { REQUIRE(success); })(1.0);
    Monitor(FirstArg(Equals(1.0)), [](const bool success) { REQUIRE(success); })(1.0, 2.0);
  }
}
