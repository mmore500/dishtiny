#pragma once
#ifndef DISH2_OPERATIONS_READNEIGHBORSTATE_HPP_INCLUDE
#define DISH2_OPERATIONS_READNEIGHBORSTATE_HPP_INCLUDE

#include <set>
#include <string>

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

/**
 * Reads a target readable state from the neighboring cell and stores it into
 * `reg[arg_0]`.
 *
 * To determine the target readable state, interprets the first bits of the
 * instruction tag as an unsigned integer then calculates the remainder of
 * integer division by the number of readable states.
 */
template< typename DishSpec >
class ReadNeighborState {

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
    typename SgplSpec::peripheral_t& peripheral
  ) noexcept {

    using as_const = const decltype(peripheral.state_node_input)&;

    const auto& neighbor_state
      = const_cast<as_const>(peripheral.state_node_input).Get();

    core.registers[ inst.args[0] ] = neighbor_state.Read( GetAddr( inst ) );

  }

  static std::string name() { return "Read Neighbor State"; }

  static size_t prevalence() { return 10; }

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

  template<typename Spec>
  static std::set<std::string> categories( const sgpl::Instruction<Spec>& ) {
    return {
      "extrinsic",
      "sensor",
      "op",
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_READNEIGHBORSTATE_HPP_INCLUDE
