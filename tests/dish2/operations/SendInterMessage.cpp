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

#include "dish2/operations/SendInterMessage.hpp"
#include "dish2/peripheral/Peripheral.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using library_t = sgpl::OpLibrary<
  dish2::SendInterMessage
>;

using sgpl_spec_t = sgpl::Spec<
  library_t,
  dish2::Peripheral
>;

using program_t = sgpl::Program< sgpl_spec_t >;


program_t make_program() {

  std::istringstream program_text{  R"( { "value0": [
    {
      "operation": "SendInterMessage",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "00000000000000000000000000000000"
    }
  ] } )" };

  program_t res;

  { cereal::JSONInputArchive archive( program_text ); archive( res ); }

  REQUIRE( res.size() == 1 );

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
  uit::Conduit<dish2::MessageMeshSpec> message_conduit;
  uit::Conduit<dish2::StateMeshSpec> state_conduit;

  using message_node_output_t = netuit::MeshNodeOutput<dish2::MessageMeshSpec>;
  using state_node_input_t = netuit::MeshNodeInput<dish2::StateMeshSpec>;
  message_node_output_t message_node_output{ message_conduit.GetInlet(), 0 };
  state_node_input_t state_node_input{ state_conduit.GetOutlet(), 0 };

  // peripheral
  dish2::Peripheral peripheral{ message_node_output, state_node_input };

  REQUIRE( cpu.TryLaunchCore() );

  REQUIRE( message_conduit.GetOutlet().Jump() == 0 );

  dish2::SendInterMessage::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );

  REQUIRE( message_conduit.GetOutlet().Jump() == 1 );
  REQUIRE( message_conduit.GetOutlet().Jump() == 0 );

  dish2::SendInterMessage::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );

  dish2::SendInterMessage::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );

  REQUIRE( message_conduit.GetOutlet().Jump() == 2 );
  REQUIRE( message_conduit.GetOutlet().Jump() == 0 );


}
