#include <ratio>
#include <sstream>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uit/fixtures/Conduit.hpp"
#include "signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "signalgp-lite/include/sgpl/library/OpLibrary.hpp"
#include "signalgp-lite/include/sgpl/operations/unary/Terminal.hpp"
#include "signalgp-lite/include/sgpl/spec/Spec.hpp"

#include "dish2/operations/AddToOwnState.hpp"
#include "dish2/peripheral/Peripheral.hpp"
#include "dish2/spec/IntraMessageMeshSpec.hpp"
#include "dish2/spec/MessageMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using Spec = dish2::Spec_default;

using library_t = sgpl::OpLibrary<
  dish2::AddToOwnState<Spec>,
  sgpl::Terminal
>;

using sgpl_spec_t = sgpl::Spec<
  library_t,
  dish2::Peripheral<Spec>
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
      "bitstring": "1111111111111111111111111111111111111111111111111111111111111110",
      "descriptors": []
    },
    {
      "operation": "Add To Own State",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
      "descriptors": []
    },
    {
      "operation": "Add To Own State",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "0000000000000000000000000000000000000000000000000000000000000001",
      "descriptors": []
    }
  ] } )" };

  program_t res;

  { cereal::JSONInputArchive archive( program_text ); archive( res ); }

  REQUIRE( res.size() == 3 );

  return res;

}

TEST_CASE("Test AddToOwnState") {

  // cpu
  using cpu_t = sgpl::Cpu< sgpl_spec_t >;
  cpu_t cpu;

  // program
  program_t program{ make_program() };
  cpu.InitializeAnchors( program );

  // peripheral
  dish2::Peripheral<Spec> peripheral{
    dish2::Peripheral<Spec>::make_dummy()
  };
  const auto& readable_state = peripheral.readable_state;
  const auto& writable_state = readable_state.template Get<
    dish2::WritableState<Spec>
  >();

  REQUIRE( cpu.TryLaunchCore() );

  for ( size_t i{}; i < readable_state.GetSize(); ++i ) {
    REQUIRE( readable_state.Read( i ) == 0 );
  }

  sgpl::Terminal::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );

  dish2::AddToOwnState<Spec>::run(
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

  dish2::AddToOwnState<Spec>::run(
    cpu.GetActiveCore(),
    program[2],
    program,
    peripheral
  );

  REQUIRE( writable_state.Get<0>() == 1 );
  REQUIRE( writable_state.Get<1>() == 1 );

  dish2::AddToOwnState<Spec>::run(
    cpu.GetActiveCore(),
    program[1],
    program,
    peripheral
  );

  REQUIRE( writable_state.Get<0>() == 2 );
  REQUIRE( writable_state.Get<1>() == 1 );

  counter = 0;
  for ( size_t i{}; i < readable_state.GetSize(); ++i ) {
    counter += readable_state.Read( i );
  }

  REQUIRE( counter == 3 );


}
