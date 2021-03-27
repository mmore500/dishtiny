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

class GroupExpirationService {

  template<typename Cell>
  static bool IsExpired( const Cell& cell, const size_t lev ) {

    using spec_t = typename Cell::spec_t;

    const size_t group_age
      = cell.template begin<dish2::KinGroupAgeWrapper<spec_t>>()->GetBuffer(
      )[ lev ];

    const size_t threshold_age = cfg.GROUP_EXPIRATION_DURATIONS()[ lev ];

    return group_age > threshold_age;

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

    for (size_t lev{}; lev < spec_t::NLEV; ++lev) {
      if ( IsExpired<Cell>( cell, lev ) && rand.P( 0.05 ) ) {
        cell.FragmentationRoutine();
        break;
      }
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_GROUPEXPIRATIONSERVICE_HPP_INCLUDE
