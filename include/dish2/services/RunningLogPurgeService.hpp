#pragma once
#ifndef DISH2_SERVICES_RUNNINGLOGPURGESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_RUNNINGLOGPURGESERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/CellAgeWrapper.hpp"
#include "../cell/cardinal_iterators/EpochWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Purges a cell's running logs.
 *
 * (Only affects data collection, not simulation logic.)
 */
struct RunningLogPurgeService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RUNNING_LOG_PURGE_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "running log purge service", "TODO", 3 };

    cell.running_logs.Purge();

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_RUNNINGLOGPURGESERVICE_HPP_INCLUDE
