#pragma once
#ifndef SGPL_OPERATIONS_FLOW_GLOBAL_JUMPIFNOT_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_GLOBAL_JUMPIFNOT_HPP_INCLUDE

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

struct JumpIfNot {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) {
    if ( !core.registers[ inst.args[0] ] ) {
      core.JumpToGlobalAnchorMatch( inst.tag );
      if ( !core.registers[ inst.args[1] ] ) core.ResetRegisters();
    }
  }

  static std::string name() { return "Global Jump If Not"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "argument b", emp::to_string( static_cast<int>( inst.args[1] ) ) },
      { "summary", "if !a, goto global anchor; if !b, clear registers" },
      { "tag bits", emp::to_string( inst.tag ) },
      {"tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) )},
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

#endif // #ifndef SGPL_OPERATIONS_FLOW_GLOBAL_JUMPIFNOT_HPP_INCLUDE
