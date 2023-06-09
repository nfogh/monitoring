#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <monitoring/monitor.hpp>
#include <monitoring/conditions/equals.hpp>
#include <monitoring/conditions/paramindex.hpp>

using namespace Monitoring;

TEST_CASE("Monitor", "[monitor]")
{
  SECTION("monitor")
  {
    Monitor(Equals(1), [](const bool success){ REQUIRE(success); })(1);
    Monitor(Equals(1.0), [](const bool success){ REQUIRE(success); })(1.0);
    Monitor(FirstParam(Equals(1.0)), [](const bool success){ REQUIRE(success); })(1.0, 2.0);
  }
}
