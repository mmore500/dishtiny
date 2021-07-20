#pragma once
#ifndef SGPL_OPERATIONS_BITWISE_RANDOMFILL_HPP_INCLUDE
#define SGPL_OPERATIONS_BITWISE_RANDOMFILL_HPP_INCLUDE

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
 * Fills register pointed to by `reg[arg_0]` with random bits chosen from a
 * uniform distribution.
 */
struct RandomFill {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {

    auto& target = core.registers[ inst.args[0] ];

    sgpl::tlrand.Get().RandFill(
      reinterpret_cast<unsigned char*>( &target ), sizeof( target )
    );

  }

  static std::string name() { return "RandomFill"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "fill a with uniformly-distributed random bits" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "bitwise",
      "calculation",
      "intrinsic",
      "op",
      "stochastic",
    };
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_BITWISE_RANDOMFILL_HPP_INCLUDE
