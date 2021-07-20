#pragma once
#ifndef DISH2_OPERATIONS_SENDINTRAMESSAGEIF_HPP_INCLUDE
#define DISH2_OPERATIONS_SENDINTRAMESSAGEIF_HPP_INCLUDE

#include <set>
#include <string>

#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../configbyroot/root_perturbation_configs.hpp"

namespace dish2 {

/**
 * Conditionally send a message to a co-cardinal.
 *
 * If `reg[arg_0]` is nonzero, generates a message tagged with the
 * instruction's tag that contains the core's current register state. Sends
 * this message to a target cardinal within the cell.
 *
 * To determine the target cardinal, sums instruction arguments then calculates
 * the remainder of integer division by the number of co-cardinals.
 */
struct SendIntraMessageIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) noexcept {

    if ( !core.registers[ inst.args[0] ] ) return;

    auto& outputs = peripheral.intra_message_node_outputs;

    const size_t num_addrs = outputs.size();
    const size_t addr = std::accumulate(
      std::begin( inst.args ), std::end( inst.args ),
      size_t{}
    ) % num_addrs;

    const auto message = std::make_tuple(
      inst.tag, core.GetRegisters()
    );
    const size_t root_id = peripheral.root_id;
    const auto& root_config = dish2::root_perturbation_configs.View( root_id );

    if ( root_config.ShouldSelfSendIntraMessage( inst.tag ) ) {
      peripheral.intra_message_selfsend_buffer.push_back( message );
    } else outputs[ addr ].TryPut( message );

  }

  static std::string name() { return "Send Intra-Cell Message"; }

  static size_t prevalence() { return 5; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "if a, send message another cardinal within cell" },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories( const sgpl::Instruction<Spec>& ) {
    return {
      "intrinsic",
      "flow",
      "op",
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_SENDINTRAMESSAGEIF_HPP_INCLUDE
