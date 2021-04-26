#pragma once
#ifndef SGPL_OPERATIONS_FLOW_GLOBAL_REGULATORADJ_HPP_INCLUDE
#define SGPL_OPERATIONS_FLOW_GLOBAL_REGULATORADJ_HPP_INCLUDE

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

template< size_t JUMP_TABLE_IDX=0 >
struct RegulatorAdj {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) {

    auto& jump_table = core.GetGlobalJumpTable( JUMP_TABLE_IDX );
    for ( const auto uid : jump_table.MatchRaw(inst.tag) ) {
      // (+) values down regulate
      // (-) values up regulate
      jump_table.AdjRegulator(
        uid,
        core.registers[ inst.args[0] ]
      );
    }

  }

  static std::string name() {
    if constexpr ( JUMP_TABLE_IDX == 0 ) {
      return "Adjust Global Regulator";
    } else if constexpr ( JUMP_TABLE_IDX == 1 ) {
      return "Adjust Secondary Global Regulator";
    } else return emp::to_string(
      "Adjust ", JUMP_TABLE_IDX, "iary Global Regulator"
    );
  }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    using tag_t = typename Spec::tag_t;

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "adjust global regulator value by a" },
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
      "regulation",
      "global regulation",
    };
  }

};

} // namespace global
} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_FLOW_GLOBAL_REGULATORADJ_HPP_INCLUDE
