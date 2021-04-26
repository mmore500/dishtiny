#pragma once
#ifndef SGPL_OPERATIONS_FLOW_LOCAL_JUMPIF_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_LOCAL_JUMPIF_HPP_INCLUDE

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

struct JumpIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) {
    if ( core.registers[ inst.args[0] ] ) {
      core.JumpToLocalAnchorMatch( inst.tag );
    }
  }

  static std::string name() { return "Local Jump If"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
      { "summary", "if a, goto tag match local anchor" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "flow",
      "local flow",
      "intrinsic",
      "op",
    };
  }

};

} // namespace local
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_LOCAL_JUMPIF_HPP_INCLUDE
