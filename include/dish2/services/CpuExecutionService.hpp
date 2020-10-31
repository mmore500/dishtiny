#pragma once
#ifndef DISH2_SERVICES_CPUEXECUTIONSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CPUEXECUTIONSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/execute_cpu.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

struct CpuExecutionService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.CPU_EXECUTION_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    if ( cell.genome->program.empty() ) return;

    using sgpl_spec_t = typename Spec::sgpl_spec_t;

    for (size_t rep = 0; rep < dish2::cfg.HARDWARE_EXECUTION_ROUNDS(); ++rep) {
      // TODO should these be shuffled?
      for (auto& cardinal : cell.cardinals) sgpl::execute_cpu<sgpl_spec_t>(
        dish2::cfg.HARDWARE_EXECUTION_CYCLES(),
        cardinal.cpu,
        cell.genome->program,
        cardinal.peripheral
      );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CPUEXECUTIONSERVICE_HPP_INCLUDE
