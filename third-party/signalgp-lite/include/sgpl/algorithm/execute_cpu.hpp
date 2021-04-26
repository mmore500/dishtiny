#pragma once
#ifndef SGPL_ALGORITHM_EXECUTE_CPU_HPP_INCLUDE
#define SGPL_ALGORITHM_EXECUTE_CPU_HPP_INCLUDE

#include <tuple>

#include "../hardware/Cpu.hpp"
#include "../program/Program.hpp"
#include "../spec/Spec.hpp"
#include "../utility/EmptyType.hpp"

#include "execute_core.hpp"

namespace sgpl {

template<typename Spec>
void execute_cpu(
  const size_t cycles,
  sgpl::Cpu<Spec>& state,
  const sgpl::Program<Spec>& program,
  typename Spec::peripheral_t& peripheral
) {

  emp_assert( program.size() );

  for (size_t i{}; i < cycles && state.HasActiveCore(); ++i) {

    auto& core = state.GetActiveCore();
    const size_t num_cycles = execute_core<Spec>(core, program, peripheral);
    state.AdvanceCycleClock( num_cycles );
    if ( core.HasTerminated() ) state.KillActiveCore();

    state.TryActivateNextCore();
  }

}

template<typename Spec=sgpl::Spec<>>
void execute_cpu(
  const size_t cycles,
  sgpl::Cpu<Spec>& state,
  const sgpl::Program<Spec>& program
) {

  using peripheral_t = typename Spec::peripheral_t;
  peripheral_t peripheral;

  execute_cpu<Spec>( cycles, state, program, peripheral );

}

} // namespace sgpl

#endif // #ifndef SGPL_ALGORITHM_EXECUTE_CPU_HPP_INCLUDE
