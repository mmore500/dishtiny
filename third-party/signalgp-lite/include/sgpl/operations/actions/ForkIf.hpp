#pragma once
#ifndef SGPL_OPERATIONS_ACTIONS_FORKIF_HPP_INCLUDE
#define SGPL_OPERATIONS_ACTIONS_FORKIF_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {

/**
 * If \texttt{reg[arg\_0]} is nonzero, registers a request to activate a new core with the module best-matching the current instruction's tag.
 *
 * These fork requests are only handled when the current core terminates. Each
 * core may only register 3 fork requests.
 */
struct ForkIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {
    // TODO allow registers to be preserved during fork
    if ( core.registers[ inst.args[0] ] ) core.RequestFork( inst.tag );
  }

  static std::string name() { return "Fork If"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
      {"summary", "if a, submit fork request (processed when core terminates)"},
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "intrinsic",
      "global flow",
      "flow",
      "op",
    };
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_ACTIONS_FORKIF_HPP_INCLUDE
