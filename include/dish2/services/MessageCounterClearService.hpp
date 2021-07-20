#pragma once
#ifndef DISH2_SERVICES_MESSAGECOUNTERCLEARSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_MESSAGECOUNTERCLEARSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/IncomingInterMessageCounterWrapper.hpp"
#include "../cell/cardinal_iterators/IncomingIntraMessageCounterWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Intermittently resets introspective message count state.
 */
struct MessageCounterClearService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq
      = dish2::cfg.INTER_MESSAGE_COUNTER_CLEAR_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "message counter clear service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    std::fill(
      cell.template begin<dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      cell.template end< dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      0
    );

    std::fill(
      cell.template begin<dish2::IncomingIntraMessageCounterWrapper<spec_t> >(),
      cell.template end< dish2::IncomingIntraMessageCounterWrapper<spec_t> >(),
      0
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_MESSAGECOUNTERCLEARSERVICE_HPP_INCLUDE
