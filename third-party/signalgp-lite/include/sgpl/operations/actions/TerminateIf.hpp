#pragma once
#ifndef SGPL_OPERATIONS_ACTIONS_TERMINATEIF_HPP_INCLUDE
#define SGPL_OPERATIONS_ACTIONS_TERMINATEIF_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {

struct TerminateIf {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>& program,
    typename Spec::peripheral_t&
  ) {
    if ( core.registers[ inst.args[0] ] ) core.Terminate();
  }

  static std::string name() { return "Terminate If"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "if a, terminate core" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "intrinsic",
      "flow",
      "op",
    };
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_ACTIONS_TERMINATEIF_HPP_INCLUDE
