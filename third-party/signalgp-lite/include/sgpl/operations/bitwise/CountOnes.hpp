#pragma once
#ifndef SGPL_OPERATIONS_BITWISE_COUNTONES_HPP_INCLUDE
#define SGPL_OPERATIONS_BITWISE_COUNTONES_HPP_INCLUDE

#include <bitset>
#include <cstddef>
#include <cstring>
#include <map>
#include <set>
#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {

/**
 * Counts the number of bits set in `reg[arg_1]` and stores the result in
 * `reg[arg_0]`.
 */
struct CountOnes {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {
    const size_t a = inst.args[0], b = inst.args[1];

    static_assert( sizeof(core.registers[a]) <= sizeof(size_t) );
    size_t as_size_t;

    std::memcpy(
      &as_size_t,
      &core.registers[a],
      sizeof( core.registers[a] )
    );

    constexpr size_t num_bits = sizeof(core.registers[a]) * 8;
    core.registers[b] = std::bitset<num_bits>{ as_size_t }.count();
  }

  static std::string name() { return "Count Ones"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "argument b", emp::to_string( static_cast<int>( inst.args[1] ) ) },
      { "summary", "a = popcnt( b )" },
    };
  }

  template<typename Spec>
  static std::set<std::string> categories(const sgpl::Instruction<Spec>&) {
    return {
      "bitwise",
      "calculation",
      "intrinsic",
      "op",
    };
  }

};

} // namespace sgpl

#endif // #ifndef SGPL_OPERATIONS_BITWISE_COUNTONES_HPP_INCLUDE
