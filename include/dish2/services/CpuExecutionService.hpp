#pragma once
#ifndef DISH2_SERVICES_CPUEXECUTIONSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CPUEXECUTIONSERVICE_HPP_INCLUDE

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

    const dish2::LogScope guard{ "cpu execution service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;
    using sgpl_spec_t = typename spec_t::sgpl_spec_t;

    if ( cell.genome->program.empty() ) return;

    for (size_t rep = 0; rep < dish2::cfg.HARDWARE_EXECUTION_ROUNDS(); ++rep) {

      auto& cardinals = cell.cardinals;

      thread_local emp::vector< decltype( cardinals.begin()) > shuffler;
      shuffler.resize( cardinals.size() );
      std::iota(
        std::begin( shuffler ), std::end( shuffler ), std::begin( cardinals )
      );
      emp::Shuffle( sgpl::tlrand.Get(), shuffler );

      for (auto cardinal_it : shuffler) sgpl::execute_cpu<sgpl_spec_t>(
        dish2::cfg.HARDWARE_EXECUTION_CYCLES(),
        cardinal_it->cpu,
        cell.genome->program,
        cardinal_it->peripheral
      );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CPUEXECUTIONSERVICE_HPP_INCLUDE
