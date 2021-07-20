#pragma once
#ifndef SGPL_OPERATIONS_UNARY_TERMINAL_HPP_INCLUDE
#define SGPL_OPERATIONS_UNARY_TERMINAL_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {

/**
 * Stores a genetically-encoded value to `reg[arg_0]`.
 *
 * This value is determined deterministically using the instruction's tag.
 */
class Terminal {

  template<typename Spec>
  static double map_between_plusminus_one(
    const typename Spec::tag_t& tag
  ) noexcept {

    constexpr double max = 1.0;
    constexpr double min = -1.0;
    constexpr double max_double = Spec::tag_t::MaxDouble();

    return  (tag.GetDouble() / max_double) * (max - min) + min;

  }

  template<typename Spec>
  static bool is_odd(const typename Spec::tag_t& tag) noexcept {
    return tag.Get(0);
  }

  static double map_up( const double plusminus_unit_val ) noexcept {
    emp_assert( plusminus_unit_val != 0 );

    return 1.0 / (
      plusminus_unit_val * plusminus_unit_val * plusminus_unit_val
    );

  }

  template<typename Spec>
  static float map_tag( const typename Spec::tag_t& tag ) noexcept {
    return is_odd<Spec>( tag )
      ? map_up( map_between_plusminus_one<Spec>(tag) )
      : map_between_plusminus_one<Spec>( tag )
    ;
  }

public:

  // writes a genetically-encoded value into a register.
  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) {

    const auto& tag = inst.tag;

    core.registers[ inst.args[0] ] = map_tag<Spec>( tag );

  }

  static std::string name() { return "Terminal"; }

  static size_t prevalence() { return 50; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "a = value" },
      { "value", emp::to_string( map_tag<Spec>( inst.tag ) ) },
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

#endif // #ifndef SGPL_OPERATIONS_UNARY_TERMINAL_HPP_INCLUDE
