#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_CPUEXECUTIONSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_CPUEXECUTIONSERVICE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/algorithm/execute_cpu.hpp"

#include "../../config/cfg.hpp"

#include "../cardinal_iterators/CpuWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::CpuExecutionService() {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  for (size_t rep = 0; rep < dish2::cfg.HARDWARE_EXECUTION_ROUNDS(); ++rep) {
    // TODO should these be shuffled?
    for (auto& cardinal : cardinals) sgpl::execute_cpu<sgpl_spec_t>(
      dish2::cfg.HARDWARE_EXECUTION_CYCLES(),
      cardinal.cpu,
      genome->program,
      cardinal.peripheral
    );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_CPUEXECUTIONSERVICE_HPP_INCLUDE
