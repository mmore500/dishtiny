#pragma once
#ifndef SGPL_OPERATIONS_FLOW_LOCAL_ANCHOR_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_LOCAL_ANCHOR_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/hash_namify.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {
namespace local {

/**
 * Marks a program location local jump instructions may route to.

 * Local jump instructions enable conditionals and looping within modules. This
 * program location is tagged with the instruction's tag.
 *
 * As described in `dish2::global::Anchor`'s docstring, this operation also
 * plays a role in determining whether global anchor instructions close a
 * module.
 */
struct Anchor {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>&,
    const sgpl::Instruction<Spec>&,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept { ; }

  static std::string name() { return "Local Anchor"; }

  static size_t prevalence() { return 20; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "summary", "register a local jump-to destination" },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "anchor",
      "local anchor",
      "flow",
      "intrinsic",
      "op",
    };
  }

};

} // namespace local
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_LOCAL_ANCHOR_HPP_INCLUDE
