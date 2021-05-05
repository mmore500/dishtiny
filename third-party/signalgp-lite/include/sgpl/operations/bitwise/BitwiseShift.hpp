#pragma once
#ifndef SGPL_OPERATIONS_BITWISE_BITWISESHIFT_HPP_INCLUDE
#define SGPL_OPERATIONS_BITWISE_BITWISESHIFT_HPP_INCLUDE

#include <cstddef>
#include <cstring>
#include <map>
#include <set>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../hardware/Core.hpp"
#include "../../program/Instruction.hpp"
#include "../../program/Program.hpp"

namespace sgpl {

struct BitwiseShift {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>& core,
    const sgpl::Instruction<Spec>& inst,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) noexcept {
    const size_t a = inst.args[0], b = inst.args[1], c = inst.args[2];

    static_assert( sizeof(core.registers[b]) <= sizeof(size_t) );
    size_t as_size_t;

    std::memcpy(
      &as_size_t,
      &core.registers[b],
      sizeof( core.registers[b] )
    );

    constexpr size_t num_bits = sizeof(core.registers[b]) * 8;

    const size_t result = ( core.registers[c] > 0 )
      ? std::bitset<num_bits>( as_size_t ).operator<<(
          uitsl::clamp_cast<size_t>( core.registers[c] )
        ).to_ulong()
      : std::bitset<num_bits>( as_size_t ).operator>>(
          uitsl::clamp_cast<size_t>( -core.registers[c] )
        ).to_ulong()
      ;

    std::memcpy(
      &core.registers[a],
      &result,
      sizeof( core.registers[a] )
    );
  }

  static std::string name() { return "Bitwise Shift"; }

  static size_t prevalence() { return 1; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "argument b", emp::to_string( static_cast<int>( inst.args[1] ) ) },
      { "argument c", emp::to_string( static_cast<int>( inst.args[2] ) ) },
      { "summary", "a = b << c" },
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

#endif // #ifndef SGPL_OPERATIONS_BITWISE_BITWISESHIFT_HPP_INCLUDE
