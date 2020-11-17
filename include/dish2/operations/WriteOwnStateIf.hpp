#pragma once
#ifndef DISH2_OPERATIONS_WRITEOWNSTATEIF_HPP_INCLUDE
#define DISH2_OPERATIONS_WRITEOWNSTATEIF_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Instruction.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"

#include "../peripheral/readable_state/writable_state/WritableState.hpp"

namespace dish2 {

template< typename DishSpec >
class WriteOwnStateIf {

  template<typename SgplSpec>
  static size_t GetAddr( const sgpl::Instruction<SgplSpec>& inst ) {
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
  ) {

    if ( !core.registers[ inst.args[1] ] ) return;

    peripheral.readable_state.template Get<
      dish2::WritableState<DishSpec>
    >().Write(
      GetAddr( inst ),
      core.registers[ inst.args[0] ]
    );

  }

  static std::string name() { return "Write Own State"; }

  static size_t prevalence() { return 5; }

  template<typename Spec>
  static auto descriptors( const sgpl::Instruction<Spec>& inst ) {

    return std::map<std::string, std::string>{
      { "argument a", emp::to_string( static_cast<int>( inst.args[0] ) ) },
      { "summary", "if b, state = a" },
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

};

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_WRITEOWNSTATEIF_HPP_INCLUDE
