#pragma once
#ifndef DISH2_SERVICES_GROUPEXPIRATIONSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_GROUPEXPIRATIONSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../enum/CauseOfDeath.hpp"

namespace dish2 {

/**
 * As group age exceeds `GROUP_EXPIRATION_DURATIONS`, with increasing
 * probability fragments cell from its kin group.
 *
 * This process kills the cell and replaces it inplace with a daughter without
 * kin ID commonality.
 */
class GroupExpirationService {

  template<typename Cell>
  static size_t AmtExpired( const Cell& cell, const size_t lev ) {

    using spec_t = typename Cell::spec_t;

    const size_t group_age
      = cell.template begin<dish2::KinGroupAgeWrapper<spec_t>>()->GetBuffer(
      )[ lev ];

    const size_t threshold_age = cfg.GROUP_EXPIRATION_DURATIONS()[ lev ];

    return group_age - std::min( group_age, threshold_age );

  }

public:

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.GROUP_EXPIRATION_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "group expiration service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    auto& rand = sgpl::tlrand.Get();

    // only expire based on outermost level
    //for (size_t lev{}; lev < spec_t::NLEV; ++lev)
    const size_t lev = spec_t::NLEV - 1; {
      const size_t expiration = AmtExpired<Cell>( cell, lev );
      const double p_fragmentation = std::min( 0.1 * expiration, 1.0 );
      if ( expiration && rand.P( p_fragmentation ) ) {
        cell.FragmentationRoutine();
        // break;
      }
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_GROUPEXPIRATIONSERVICE_HPP_INCLUDE
