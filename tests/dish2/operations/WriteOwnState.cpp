#define CATCH_CONFIG_MAIN

#include <ratio>
#include <sstream>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uit/fixtures/Conduit.hpp"
#include "signalgp-lite/include/sgpl/config/Spec.hpp"
#include "signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "signalgp-lite/include/sgpl/library/OpLibrary.hpp"
#include "signalgp-lite/include/sgpl/operations/unary/Terminal.hpp"

#include "dish2/operations/WriteOwnState.hpp"
#include "dish2/peripheral/Peripheral.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using library_t = sgpl::OpLibrary<
  dish2::WriteOwnState,
  sgpl::Terminal<std::ratio<1>, std::ratio<-1>>
>;

using sgpl_spec_t = sgpl::Spec<
  library_t,
  dish2::Peripheral
>;

using program_t = sgpl::Program< sgpl_spec_t >;


program_t make_program() {

  std::istringstream program_text{  R"( { "value0": [
    {
      "operation": "Terminal",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "11111111111111111111111111111111"
    },
    {
      "operation": "WriteOwnState",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "00000000000000000000000000000000"
    },
    {
      "operation": "WriteOwnState",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "00000000000000000100000000000000"
    }
  ] } )" };

  program_t res;

  { cereal::JSONInputArchive archive( program_text ); archive( res ); }

  REQUIRE( res.size() == 3 );

  return res;

}

TEST_CASE("Test WriteOwnState") {

  // cpu
  using cpu_t = sgpl::Cpu< sgpl_spec_t >;
  cpu_t cpu;

  // program
  program_t program{ make_program() };
  cpu.InitializeAnchors( program );

  // conduit
  uit::Conduit<dish2::StateMeshSpec> conduit;

  // peripheral
  using state_node_input_t = netuit::MeshNodeInput<dish2::StateMeshSpec>;
  dish2::Peripheral peripheral{ state_node_input_t{ conduit.GetOutlet(), 0 } };
  const auto& readable_state = peripheral.readable_state;
  const auto& writable_state = readable_state.template Get<
    dish2::WritableState
  >();

  REQUIRE( cpu.TryLaunchCore() );

  for ( size_t i{}; i < readable_state.GetSize(); ++i ) {
    REQUIRE( readable_state.Read( i ) == 0 );
  }

  sgpl::Terminal<std::ratio<1>, std::ratio<-1>>::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );

  dish2::WriteOwnState::run(
    cpu.GetActiveCore(),
    program[1],
    program,
    peripheral
  );

  REQUIRE( writable_state.Get<0>() == 1 );
  REQUIRE( writable_state.Get<1>() == 0 );

  size_t counter{};
  for ( size_t i{}; i < readable_state.GetSize(); ++i ) {
    counter += readable_state.Read( i );
  }

  REQUIRE( counter == 1 );

  dish2::WriteOwnState::run(
    cpu.GetActiveCore(),
    program[2],
    program,
    peripheral
  );

  REQUIRE( writable_state.Get<0>() == 1 );
  REQUIRE( writable_state.Get<1>() == 1 );

  counter = 0;
  for ( size_t i{}; i < readable_state.GetSize(); ++i ) {
    counter += readable_state.Read( i );
  }

  REQUIRE( counter == 2 );


}
