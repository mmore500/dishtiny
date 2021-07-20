#pragma once
#ifndef DISH2_OPERATIONS_MULTIPLYOWNSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_MULTIPLYOWNSTATE_HPP_INCLUDE

#include <set>
#include <string>

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/readable_state/writable_state/WritableState.hpp"

namespace dish2 {

/**
 * Multiplies `reg[arg_0]` by the current value in a target writable state then
 * stores the result back in to that target writable state.
 *
 * To determine the target writable state, interprets the first bits of the
 * instruction tag as an unsigned integer then calculates the remainder of
 * integer division by the number of writable states.
 */
template< typename DishSpec >
class MultiplyOwnState {

  template<typename SgplSpec>
  static size_t GetAddr( const sgpl::Instruction<SgplSpec>& inst ) noexcept {
    constexpr size_t num_addrs = dish2::WritableState< DishSpec >::GetSize();
    const size_t addr = inst.tag.GetUInt(0) % num_addrs;
    return addr;
  }

public:

  template<typename SgplSpec>
  static void run(
    sgpl::Core<SgplSpec>& core,
    const sgpl::Instruction<SgplSpec>& inst,
    const sgpl::Program<SgplSpec>&,
    typename SgplSpec::peripheral_t& peripheral
  ) noexcept {

    peripheral.readable_state.template Get<
      dish2::WritableState< DishSpec >
    >().Multiply(
      GetAddr( inst ),
      core.registers[ inst.args[0] ]
    );

  }

  static std::string name() { return "Multiply Own State"; }

  static size_t prevalence() { return 5; }

  template<typename SgplSpec>
  static auto descriptors( const sgpl::Instruction<SgplSpec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "state *= a" },
      { "target",
        dish2::WritableState< DishSpec >::GetLeafTypeName(
          GetAddr( inst )
        )
      },
      { "target index",
        emp::to_string( dish2::WritableState< DishSpec >::GetLeafIndex(
          GetAddr( inst )
        ) )
      },
    };

  }

  template<typename SgplSpec>
  static auto categories( const sgpl::Instruction<SgplSpec>& inst ) {

    const bool nop_target = dish2::WritableState< DishSpec >::GetLeafTypeName(
      GetAddr( inst )
    ).find("NopState") != std::string::npos;

    const bool apoptosis_request = dish2::WritableState< DishSpec >::GetLeafTypeName(
      GetAddr( inst )
    ).find("ApoptosisRequest") != std::string::npos;

    std::set<std::string> res{
      nop_target ? "calculation" : "actuator",
      nop_target ? "intrinsic" : "extrinsic",
      "op",
    };

    if ( apoptosis_request ) res.insert( "apoptosis" );
    return res;

  }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_MULTIPLYOWNSTATE_HPP_INCLUDE
