#pragma once
#ifndef DISH2_SERVICES_INTERMESSAGEPURGINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMESSAGEPURGINGSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../cell/cardinal_iterators/IncomingInterMessageCounterWrapper.hpp"
#include "../cell/cardinal_iterators/MessageNodeInputWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Purges excess incoming inter-cell messages that couldn't be handled due to
 * virtual core availability.
 */
struct InterMessagePurgingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.INTER_MESSAGE_LAUNCHING_SERVICE_FREQUENCY();
    // must run whether cell is alive or not to keep aggregated jumps in sync
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 1;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "inter message purging service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    uitsl::for_each(
      cell.template begin< dish2::MessageNodeInputWrapper<spec_t> >(),
      cell.template end< dish2::MessageNodeInputWrapper<spec_t> >(),
      cell.template begin<dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      []( auto& message_input, auto& message_counter ) {
        message_counter += message_input.Jump();
      }
    );

    // self-send messages are purged in InterMessageLaunchingService

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMESSAGEPURGINGSERVICE_HPP_INCLUDE
