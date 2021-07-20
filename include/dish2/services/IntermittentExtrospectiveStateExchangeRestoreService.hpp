#pragma once
#ifndef DISH2_SERVICES_INTERMITTENTEXTROSPECTIVESTATEEXCHANGERESTORESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMITTENTEXTROSPECTIVESTATEEXCHANGERESTORESERVICE_HPP_INCLUDE

#include <algorithm>
#include <numeric>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/datastructs/RingBuffer.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/execute_cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/ExtrospectiveStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

#include "IntermittentExtrospectiveStateExchangeService.hpp"

namespace dish2 {

/**
 * Used for experimental manipulations testing the fitness effect of
 * extrospective state.
 *
 * (Not part of core simulation logic.)
 */
struct IntermittentExtrospectiveStateExchangeRestoreService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq
      = dish2::cfg.INTERMITTENT_STATE_PERTURB_SERVICES_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }


  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{
      "intermittent introspective state exchange restore service", "TODO", 3
    };

    using spec_t = typename Cell::spec_t;

    auto& stash = IntermittentExtrospectiveStateExchangeService<spec_t>::stash;

    emp_assert( stash.empty() || stash.size() == cell.GetNumCardinals() );

    if ( stash.size() ) std::copy(
      std::begin( stash ),
      std::end( stash ),
      cell.template begin<dish2::ExtrospectiveStateWrapper<spec_t>>()
    );

    stash.clear();

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMITTENTEXTROSPECTIVESTATEEXCHANGERESTORESERVICE_HPP_INCLUDE
