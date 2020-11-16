#pragma once
#ifndef DISH2_OPERATIONS_BCSTINTRAMESSAGE_HPP_INCLUDE
#define DISH2_OPERATIONS_BCSTINTRAMESSAGE_HPP_INCLUDE

#include <tuple>

#include "../../../third-party/Empirical/source/tools/hash_namify.h"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

namespace dish2 {

struct BcstIntraMessage {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t& peripheral
  ) {

    for ( auto& out : peripheral.intra_message_node_outputs ) {
      out.TryPut( std::make_tuple(
        inst.tag, core.GetRegisters()
      ) );
    }

  }

  static std::string name() { return "Broadcast Intra-Cell Message"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "summary", "TODO" },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) }
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_BCSTINTRAMESSAGE_HPP_INCLUDE
