#pragma once
#ifndef DISH2_SERVICES_PUSHSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_PUSHSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/PushNodeInputWrapper.hpp"
#include "../cell/cardinal_iterators/PushNodeOutputWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Checks for incoming "push" messages, applies their functors, and dispatches
 * any resultant "push messages.
 *
 * See `push/`.
 */
struct PushService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.PUSH_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "push service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    std::for_each(
      cell.template begin< dish2::PushNodeInputWrapper<spec_t> >(),
      cell.template end< dish2::PushNodeInputWrapper<spec_t> >(),
      [&cell]( auto& input ){

        while ( input.TryStep() ) {
          const auto& message = input.Get();
          const auto res = message.ApplyTo( cell.cell_push_state );

          if ( res.has_value() ) std::for_each(
            cell.template begin< dish2::PushNodeOutputWrapper<spec_t> >(),
            cell.template end< dish2::PushNodeOutputWrapper<spec_t> >(),
            [&res]( auto& output ){ output.Put( *res ); }
          );

        }

      }
    );

    std::for_each(
      cell.template begin< dish2::PushNodeOutputWrapper<spec_t> >(),
      cell.template end< dish2::PushNodeOutputWrapper<spec_t> >(),
      []( auto& output ){ output.TryFlush(); }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_PUSHSERVICE_HPP_INCLUDE
