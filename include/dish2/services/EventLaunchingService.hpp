#pragma once
#ifndef DISH2_SERVICES_EVENTLAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_EVENTLAUNCHINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <type_traits>

#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Dispatches environmentally-managed events for each cardinal.
 */
struct EventLaunchingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.EVENT_LAUNCHING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "event launching service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;
    using event_manager_t = typename spec_t::event_manager_t;
    using event_tags_t = typename std::decay_t< decltype(
      cell.genome->event_tags
    ) >;

    for (auto& cardinal : cell.cardinals) {
      using cardinal_t = typename std::decay_t< decltype(cardinal) >;
      event_manager_t::template Dispatch< cardinal_t, event_tags_t >(
        cardinal, cell.genome->event_tags
      );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_EVENTLAUNCHINGSERVICE_HPP_INCLUDE
