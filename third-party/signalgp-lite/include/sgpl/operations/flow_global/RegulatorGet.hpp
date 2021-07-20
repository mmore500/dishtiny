#pragma once
#ifndef SGPL_OPERATIONS_FLOW_GLOBAL_REGULATORGET_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_GLOBAL_REGULATORGET_HPP_INCLUDE

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
 * Gets the regulator value of the global jump table tag that best matches this
 * instruction's tag.
 *
 * Stores the value in `reg[arg_0]`. If no tag matches, a no-op is performed.
 *
 * The regulator value gotten affects the outcome of tag lookup for module
 * activation on global jump table `JUMP_TABLE_IDX`.
 */
template< size_t JUMP_TABLE_IDX=0 >
struct RegulatorGet {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {

    auto& jump_table = core.GetGlobalJumpTable( JUMP_TABLE_IDX );
    for ( const auto uid : jump_table.MatchRaw(inst.tag) ) {
      // (+) values down regulate
      // (-) values up regulate
      core.registers[ inst.args[0] ] = jump_table.ViewRegulator( uid );
      break; // only process first value
    }

  }

  static std::string name() {
    if constexpr ( JUMP_TABLE_IDX == 0 ) {
      return "Get Global Regulator";
    } else if constexpr ( JUMP_TABLE_IDX == 1 ) {
      return "Get Secondary Global Regulator";
    } else return emp::to_string(
      "Get ", JUMP_TABLE_IDX, "iary Global Regulator"
    );
  }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "a = global regulator value" },
      { "target jump table", emp::to_string( JUMP_TABLE_IDX ) },
      { "tag bits", emp::to_string( inst.tag ) },
      { "tag moniker", emp::hash_namify( std::hash< tag_t >{}( inst.tag ) ) },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "intrinsic",
      "op",
    };
  }

};

} // namespace global
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_GLOBAL_REGULATORGET_HPP_INCLUDE
