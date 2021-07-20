#pragma once
#ifndef DISH2_SERVICES_APOPTOSISSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_APOPTOSISSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/ApoptosisRequestWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../enum/CauseOfDeath.hpp"

namespace dish2 {

/**
 * If any cardinals have requested apoptosis, do death routine on the cell.
 */
struct ApoptosisService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.APOPTOSIS_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "apoptosis service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    if ( std::any_of(
      cell.template begin<dish2::ApoptosisRequestWrapper<spec_t>>(),
      cell.template end<dish2::ApoptosisRequestWrapper<spec_t>>(),
      std::identity
    ) ) cell.DeathRoutine( dish2::CauseOfDeath::apoptosis );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_APOPTOSISSERVICE_HPP_INCLUDE
