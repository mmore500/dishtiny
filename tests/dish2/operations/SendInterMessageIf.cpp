#include <ratio>
#include <sstream>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uit/fixtures/Conduit.hpp"
#include "signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "signalgp-lite/include/sgpl/library/OpLibrary.hpp"
#include "signalgp-lite/include/sgpl/operations/unary/Terminal.hpp"
#include "signalgp-lite/include/sgpl/spec/Spec.hpp"

#include "dish2/operations/SendInterMessageIf.hpp"
#include "dish2/peripheral/Peripheral.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using Spec = dish2::Spec_default;

using library_t = sgpl::OpLibrary<
  dish2::SendInterMessageIf,
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
      "operation": "Send Inter-Cell Message",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
      "descriptors": []
    }
  ] } )" };

  program_t res;

  { cereal::JSONInputArchive archive( program_text ); archive( res ); }

  REQUIRE( res.size() == 2 );

  return res;

}

TEST_CASE("Test SendInterMessageIf") {

  // cpu
  using cpu_t = sgpl::Cpu< sgpl_spec_t >;
  cpu_t cpu;

  // program
  program_t program{ make_program() };
  cpu.InitializeAnchors( program );

  // conduit
  uit::Conduit<dish2::MessageMeshSpec<Spec>> message_conduit;
  uit::Conduit<dish2::StateMeshSpec<Spec>> state_conduit;

  using intra_message_mesh_spec_t = dish2::IntraMessageMeshSpec< Spec >;
  using intra_message_node_outputs_t
    = typename netuit::MeshNode<intra_message_mesh_spec_t>::outputs_t;
  intra_message_node_outputs_t intra_message_node_outputs;
  using message_node_output_t = netuit::MeshNodeOutput<
    dish2::MessageMeshSpec< Spec >
  >;
  using state_node_input_t = netuit::MeshNodeInput<
    dish2::StateMeshSpec< Spec >
  >;
  message_node_output_t message_node_output{ message_conduit.GetInlet(), 0 };
  state_node_input_t state_node_input{ state_conduit.GetOutlet(), 0 };

  // peripheral
  dish2::Peripheral peripheral{
    intra_message_node_outputs, message_node_output, state_node_input
  };

  REQUIRE( cpu.TryLaunchCore() );

  sgpl::Terminal::run(
    cpu.GetActiveCore(),
    program[0],
    program,
    peripheral
  );

  REQUIRE( message_conduit.GetOutlet().Jump() == 0 );

  dish2::SendInterMessageIf::run(
    cpu.GetActiveCore(),
    program[1],
    program,
    peripheral
  );

  REQUIRE( message_conduit.GetOutlet().Jump() == 1 );
  REQUIRE( message_conduit.GetOutlet().Jump() == 0 );

  dish2::SendInterMessageIf::run(
    cpu.GetActiveCore(),
    program[1],
    program,
    peripheral
  );

  dish2::SendInterMessageIf::run(
    cpu.GetActiveCore(),
    program[1],
    program,
    peripheral
  );

  REQUIRE( message_conduit.GetOutlet().Jump() == 2 );
  REQUIRE( message_conduit.GetOutlet().Jump() == 0 );


}
