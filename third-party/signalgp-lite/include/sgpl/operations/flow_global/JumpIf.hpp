#pragma once
#ifndef SGPL_OPERATIONS_FLOW_GLOBAL_JUMPIF_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_GLOBAL_JUMPIF_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {
namespace global {

/**
 * Conditionally jumps core to execute a new module.
 *
 * Jumps the current core to a global anchor that matches the instruction tag
 * if `reg[arg_0]` is nonzero. If `reg[arg_1]` is nonzero, resets registers.
 */
struct JumpIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {
    if ( core.registers[ inst.args[0] ] ) {
      core.JumpToGlobalAnchorMatch( inst.tag );
      if ( core.registers[ inst.args[1] ] ) core.ResetRegisters();
    }
  }

  static std::string name() { return "Global Jump If"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "argument b", emp::to_string( static_cast<int>( inst.args[1] ) ) },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
      {"summary", "if a, goto tag match global anchor; if b, clear registers"},
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "flow",
      "global flow",
      "intrinsic",
      "op",
    };
  }

};

} // namespace global
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_GLOBAL_JUMPIF_HPP_INCLUDE
