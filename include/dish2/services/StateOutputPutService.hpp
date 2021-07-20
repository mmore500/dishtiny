#pragma once
#ifndef DISH2_SERVICES_STATEOUTPUTPUTSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_STATEOUTPUTPUTSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/ReadableStateWrapper.hpp"
#include "../cell/cardinal_iterators/StateNodeOutputWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Dispatches a copy of each cardinal's current readable state to each
 * cardinal's neighbor cell.
 */
struct StateOutputPutService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.STATE_OUTPUT_PUT_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "state output put service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    uitsl::for_each(
      cell.template begin<dish2::StateNodeOutputWrapper<spec_t>>(),
      cell.template end<dish2::StateNodeOutputWrapper<spec_t>>(),
      cell.template begin<dish2::ReadableStateWrapper<spec_t>>(),
      []( auto& output, const auto& readable_state ){
        output.TryPut( readable_state );
      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_STATEOUTPUTPUTSERVICE_HPP_INCLUDE
