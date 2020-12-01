#pragma once
#ifndef DISH2_OPERATIONS_SENDINTERMESSAGEIF_HPP_INCLUDE
#define DISH2_OPERATIONS_SENDINTERMESSAGEIF_HPP_INCLUDE

#include <tuple>

#include "../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct SendInterMessageIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    if ( !core.registers[ inst.args[0] ] ) return;

    peripheral.message_node_output.TryPut( std::make_tuple(
      inst.tag, core.GetRegisters()
    ) );

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

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_SENDINTERMESSAGEIF_HPP_INCLUDE
