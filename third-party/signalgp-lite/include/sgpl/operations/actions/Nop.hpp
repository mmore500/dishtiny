#pragma once
#ifndef SGPL_OPERATIONS_ACTIONS_NOP_HPP_INCLUDE
#define SGPL_OPERATIONS_ACTIONS_NOP_HPP_INCLUDE

#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"
#include "../../utility/ThreadLocalRandom.hpp"

namespace sgpl {

/**
 * Performs no operation for one virtual CPU cycle.
 *
 * Advances the RNG engine `NumRngTouches` times. (Important to nop-out
 * operations that perform one RNG touch without causing side effects.)
 */
template< size_t NumRngTouches=0, size_t Prevalence=1 >
struct Nop {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>&,
    const sgpl::Instruction<Spec>&,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {
    for (size_t i{}; i < NumRngTouches; ++i ) {
      sgpl::tlrand.Get().StepEngine();
    }
  }

  static std::string name() { return emp::to_string("Nop-", NumRngTouches); }

  static size_t prevalence() { return Prevalence; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "summary", "perform no operation" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "nop",
    };
  }


};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_ACTIONS_NOP_HPP_INCLUDE
