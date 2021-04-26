#include <algorithm>
#include <ratio>

#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/polyfill/identity.hpp"
#include "Empirical/include/emp/math/Random.hpp"

#include "sgpl/algorithm/execute_cpu.hpp"
#include "sgpl/hardware/Cpu.hpp"
#include "sgpl/program/Program.hpp"
#include "sgpl/spec/Spec.hpp"

using spec_t = sgpl::Spec<>;

TEST_CASE("Test execute_cpu") {

  emp::Random rand;

  sgpl::Cpu<spec_t> cpu;
  sgpl::Program<spec_t> program{ 100 };
  cpu.InitializeAnchors( program );

  REQUIRE( cpu.GetNumBusyCores() == 0 );

  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( cpu.GetNumBusyCores() == 1 );

  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( cpu.GetNumBusyCores() == 3 );

  // TODO flesh out stub test
  sgpl::execute_cpu<spec_t>( 10 * std::mega::num, cpu, program );

}
