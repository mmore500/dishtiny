#pragma once
#ifndef DISH2_SERVICES_INTERMITTENTCPURESETSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMITTENTCPURESETSERVICE_HPP_INCLUDE

#include <numeric>

#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/execute_cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

struct IntermittentCpuResetService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.INTERMITTENT_CPU_RESET_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "intermittent cpu reset service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    if (
      cell.genome->config_customizations.ShouldResetCpu()
    ) std::for_each(
      cell.template begin<dish2::CpuWrapper<spec_t>>(),
      cell.template end<dish2::CpuWrapper<spec_t>>(),
      [&cell]( auto& cpu ){
        cpu.Reset();
        cpu.InitializeAnchors( cell.genome->program );
      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMITTENTCPURESETSERVICE_HPP_INCLUDE
