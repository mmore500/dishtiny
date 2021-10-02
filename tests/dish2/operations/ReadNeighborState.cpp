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

#include "dish2/operations/ReadNeighborState.hpp"
#include "dish2/peripheral/Peripheral.hpp"
#include "dish2/spec/IntraMessageMeshSpec.hpp"
#include "dish2/spec/MessageMeshSpec.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/spec/StateMeshSpec.hpp"

using Spec = dish2::Spec_default;

using library_t = sgpl::OpLibrary<
  dish2::ReadNeighborState<Spec>
>;

using sgpl_spec_t = sgpl::Spec<
  library_t,
  dish2::Peripheral<Spec>
>;

using program_t = sgpl::Program< sgpl_spec_t >;


program_t make_program() {

  std::istringstream program_text{  R"( { "value0": [
    {
      "operation": "Read Neighbor State",
      "args": {
        "value0": 0,
        "value1": 0,
        "value2": 0
      },
      "bitstring": "0000000000000000000000000000000000000000000000000000000000000000",
      "descriptors": []
    },
    {
      "operation": "Read Neighbor State",
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

TEST_CASE("Test ReadNeighborState") {

  // set up cpu
  using cpu_t = sgpl::Cpu< sgpl_spec_t >;
  cpu_t cpu;

  // set up program
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

  REQUIRE( state_conduit.GetInlet().TryPut( readable_state ) );
  REQUIRE( peripheral.state_node_input.JumpGet() == readable_state );

  // registers should be zero-initialized
  REQUIRE( cpu.TryLaunchCore() );
  REQUIRE( std::none_of(
    std::begin( cpu.GetActiveCore().registers ),
    std::end( cpu.GetActiveCore().registers ),
    std::identity
  ) );

  // read from first readable_state position
  dish2::ReadNeighborState<Spec>::run(
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
  dish2::ReadNeighborState<Spec>::run(
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
