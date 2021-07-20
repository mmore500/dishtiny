#pragma once
#ifndef DISH2_OPERATIONS_READOWNSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_READOWNSTATE_HPP_INCLUDE

#include <set>
#include <string>

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Reads a target readable state and stores it into `reg[arg_0]`.
 *
 * To determine the target readable state, interprets the first bits of the
 * instruction tag as an unsigned integer then calculates the remainder of
 * integer division by the number of readable states.
 */
template< typename DishSpec >
class ReadOwnState {

  template<typename SgplSpec>
  static size_t GetAddr( const sgpl::Instruction<SgplSpec>& inst ) noexcept {
    constexpr size_t num_addrs = dish2::ReadableState< DishSpec >::GetSize();
    const size_t addr = inst.tag.GetUInt(0) % num_addrs;
    return addr;
  }

public:

  template<typename SgplSpec>
  static void run(
    sgpl::Core<SgplSpec>& core,
    const sgpl::Instruction<SgplSpec>& inst,
    const sgpl::Program<SgplSpec>&,
    const typename SgplSpec::peripheral_t& peripheral
  ) noexcept {

    core.registers[ inst.args[0] ] = peripheral.readable_state.Read(
      GetAddr(inst)
    );

  }

  static std::string name() { return "Read Own State"; }

  static size_t prevalence() { return 20; }

  template<typename SgplSpec>
  static auto descriptors( const sgpl::Instruction<SgplSpec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "a = state" },
      { "target",
        dish2::ReadableState< DishSpec >::GetLeafTypeName(
          GetAddr( inst )
        )
      },
      { "target index",
        emp::to_string( dish2::ReadableState< DishSpec >::GetLeafIndex(
          GetAddr( inst )
        ) )
      },
    };

  }

  template<typename SgplSpec>
  static auto categories( const sgpl::Instruction<SgplSpec>& inst ) {

    const bool nop_target = dish2::ReadableState< DishSpec >::GetLeafTypeName(
      GetAddr( inst )
    ).find("NopState") != std::string::npos;

    return std::set<std::string>{
      nop_target ? "calculation" : "sensor",
      nop_target ? "intrinsic" : "extrinsic",
      "op",
    };

  }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_READOWNSTATE_HPP_INCLUDE
