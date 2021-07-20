#pragma once
#ifndef SGPL_OPERATIONS_FLOW_GLOBAL_ANCHOR_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_GLOBAL_ANCHOR_HPP_INCLUDE

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
 * Marks a module-begin position.
 *
 * Based on tag-lookup, new cores or global jump instructions may set the
 * program counter to this instruction's program position.
 *
 * This instruction can also mark a module-end position --- executing this
 * instruction can terminate the executing core. If no local anchor instruction
 * is present between the current global anchor instruction and the preceding
 * global anchor instruction, this operation will not terminate the executing
 * core. (This way, several global anchors may lead into the same module.)
 *
 * However, if a local anchor instruction is present between the current global
 * anchor instruction and the preceding global anchor instruction, this
 * operation will terminate the executing core. Local jump instructions will
 * only consider local anchors between the preceding global anchor and the
 * subsequent global anchor instruction.
 */
struct Anchor {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>&,
    const sgpl::Program<Spec>& program,
    typename Spec::peripheral_t& peripheral
  ) noexcept {
    // when launching a new core, local anchors won't be present
    // when reaching a global anchor
    // without encountering local anchors yet,
    // local anchors will also still be empty
    if ( core.HasLocalAnchors() ) core.Terminate();
    else core.LoadLocalAnchors( program );
  }

  static std::string name() { return "Global Anchor"; }

  static size_t prevalence() { return 15; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "summary", "register a global jump-to destination, maybe terminate" },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "anchor",
      "global anchor",
      "flow",
      "global flow",
      "intrinsic",
      "op",
    };
  }

};

} // namespace global
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_GLOBAL_ANCHOR_HPP_INCLUDE
