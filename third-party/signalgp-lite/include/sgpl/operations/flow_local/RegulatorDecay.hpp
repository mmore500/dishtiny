#pragma once
#ifndef SGPL_OPERATIONS_FLOW_LOCAL_REGULATORDECAY_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_LOCAL_REGULATORDECAY_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {
namespace local {

/**
 * Ages the regulator decay countdown of local jump table tags matching this
 * instruction's tag by the amount `reg[arg_0]`.
 *
 * When a regulator ages past a threshold, it is reset to default. If
 * `reg[arg_0]` is negative, this operation can forestall decay.
 */
struct RegulatorDecay {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {

    for ( const auto uid : core.GetLocalJumpTable().MatchRaw(inst.tag) ) {
      core.GetLocalJumpTable().DecayRegulator(
        uid,
        uitsl::clamp_cast<int32_t>( core.registers[ inst.args[0] ] )
      );
    }

  }

  static std::string name() { return "Decay Local Regulator"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "decay local regulator value by register a" },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "intrinsic",
      "op",
      "regulation",
      "local regulation",
    };
  }

};

} // namespace local
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_LOCAL_REGULATORDECAY_HPP_INCLUDE
