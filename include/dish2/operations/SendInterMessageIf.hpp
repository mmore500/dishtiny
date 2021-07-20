#pragma once
#ifndef DISH2_OPERATIONS_SENDINTERMESSAGEIF_HPP_INCLUDE
#define DISH2_OPERATIONS_SENDINTERMESSAGEIF_HPP_INCLUDE

#include <set>
#include <string>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../configbyroot/root_perturbation_configs.hpp"

namespace dish2 {

/**
 * Conditionally send a message to this cardinal's neighbor cell.
 *
 * If `reg[arg_0]` is nonzero, generates a message tagged with the
 * instruction's tag that contains the core's current register state.
 * Sends this message to the neighboring cell.
 */
struct SendInterMessageIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    if ( !core.registers[ inst.args[0] ] ) return;

    const auto message = std::make_tuple(
      inst.tag, core.GetRegisters()
    );
    const size_t root_id = peripheral.root_id;
    const auto& root_config = dish2::root_perturbation_configs.View( root_id );

    if ( root_config.ShouldSelfSendInterMessage( inst.tag ) ) {
      peripheral.inter_message_selfsend_buffer.push_back( message );
    } else peripheral.message_node_output.TryPut( message );

  }

  static std::string name() { return "Send Inter-Cell Message"; }

  static size_t prevalence() { return 5; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "if a, send message to neighbor cell" },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories( const sgpl::Instruction<Spec>& ) {
    return {
      "actuator",
      "extrinsic",
      "intermessaging",
      "op",
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_SENDINTERMESSAGEIF_HPP_INCLUDE
