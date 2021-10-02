#include <algorithm>
#include <ratio>
#include <sstream>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uit/fixtures/Conduit.hpp"
#include "conduit/include/uitsl/polyfill/identity.hpp"
#include "signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "signalgp-lite/include/sgpl/library/OpLibrary.hpp"
#include "signalgp-lite/include/sgpl/spec/Spec.hpp"

#include "dish2/operations/ReadOwnState.hpp"
#include "dish2/peripheral/Peripheral.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using Spec = dish2::Spec_default;

using library_t = sgpl::OpLibrary<
  dish2::ReadOwnState<Spec>
>;

using sgpl_spec_t = sgpl::Spec<
  library_t,
  dish2::Peripheral<Spec>
>;

using program_t = sgpl::Program< sgpl_spec_t >;


program_t make_program() {

  std::istringstream program_text{  R"( { "value0": [
    {
      "operation": "Read Own State",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
      "descriptors": []
    },
    {
      "operation": "Read Own State",
      "args": {
        "value0": 1,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "0000000000000000000000000000000000000000000000000000000000000001",
      "descriptors": []
    }
  ] } )" };

  program_t res;

  { cereal::JSONInputArchive archive( program_text ); archive( res ); }

  REQUIRE( res.size() == 2 );

  return res;

}

TEST_CASE("Test ReadOwnState") {

  // set up cpu
  using cpu_t = sgpl::Cpu< sgpl_spec_t >;
  cpu_t cpu;

  // set up program
  program_t program{ make_program() };
  cpu.InitializeAnchors( program );

  // peripheral
  dish2::Peripheral<Spec> peripheral{
    dish2::Peripheral<Spec>::make_dummy()
  };
  auto& readable_state = peripheral.readable_state;

  // readable state should be zero-initialized
  for ( size_t i{}; i < readable_state.GetSize(); ++i ) {
    REQUIRE( readable_state.Read( i ) == 0 );
  }

  // set up test values to read in to cpu registers
  readable_state.Get<0>() = 42;
  REQUIRE( readable_state.Get<0>() == 42 );

  readable_state.Get<1>() = 101;
  REQUIRE( readable_state.Get<1>() == 101 );

  // registers should be zero-initialized
  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( std::none_of(
    std::begin( cpu.GetActiveCore().registers ),
    std::end( cpu.GetActiveCore().registers ),
    std::identity
  ) );

  // read from first readable_state position
  dish2::ReadOwnState< Spec >::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );
  REQUIRE( cpu.GetActiveCore().registers[0] == 42 );
  REQUIRE( std::accumulate(
    std::begin( cpu.GetActiveCore().registers ),
    std::end( cpu.GetActiveCore().registers ),
    0.0f
  ) == 42.0f );

  // read from second readable_state position
  dish2::ReadOwnState< Spec >::run(
    cpu.GetActiveCore(),
    program[1],
    program,
    peripheral
  );
  REQUIRE( cpu.GetActiveCore().registers[0] == 42 );
  REQUIRE( cpu.GetActiveCore().registers[1] == 101 );
  REQUIRE( std::accumulate(
    std::begin( cpu.GetActiveCore().registers ),
    std::end( cpu.GetActiveCore().registers ),
    0.0f
  ) == 143.0f );

}
