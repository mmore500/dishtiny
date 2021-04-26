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

struct Anchor {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>& program,
    typename Spec::peripheral_t& peripheral
  ) {
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
