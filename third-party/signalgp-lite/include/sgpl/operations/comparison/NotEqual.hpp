#pragma once
#ifndef SGPL_OPERATIONS_COMPARISON_NOTEQUAL_HPP_INCLUDE
#define SGPL_OPERATIONS_COMPARISON_NOTEQUAL_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {

/**
 * Checks whether `reg[arg_1]` is not equal to `reg[arg_2]` and stores the
 * result in `reg[arg_0]`.
 */
struct NotEqual {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {
    const size_t a = inst.args[0], b = inst.args[1], c = inst.args[2];
    core.registers[a] = core.registers[b] != core.registers[c];
  }

  static std::string name() { return "Not Equal"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "argument b", emp::to_string( static_cast<int>( inst.args[1] ) ) },
      { "argument c", emp::to_string( static_cast<int>( inst.args[2] ) ) },
      { "summary", "a = (b != c)" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "calculation",
      "intrinsic",
      "op",
    };
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_COMPARISON_NOTEQUAL_HPP_INCLUDE
