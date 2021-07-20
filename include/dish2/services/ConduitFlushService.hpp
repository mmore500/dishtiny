#pragma once
#ifndef DISH2_SERVICES_CONDUITFLUSHSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CONDUITFLUSHSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Flushes each cardinals' output conduits.
 *
 * Depending on conduit implementation, this may dispatch inter-process and
 * inter-thread communication.
 */
struct ConduitFlushService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.CONDUIT_FLUSH_SERVICE_FREQUENCY();
    // must run whether cell is alive or not to keep aggregated flushes in sync
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "conduit flush service", "TODO", 3 };

    for (auto& cardinal : cell.cardinals) {
      cardinal.genome_node_output.TryFlush();
      cardinal.message_node_output.TryFlush();
      cardinal.cardinal_quorum_state.output.TryFlush();
      cardinal.resource_node_output.TryFlush();
      cardinal.state_node_output.TryFlush();
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CONDUITFLUSHSERVICE_HPP_INCLUDE
