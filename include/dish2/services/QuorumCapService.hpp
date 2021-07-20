#pragma once
#ifndef DISH2_SERVICES_QUORUMCAPSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_QUORUMCAPSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Enforces a hard cap on kin group size.
 *
 * Probabilistically kills cells in kin groups where quorum count exceeds
 * `QUORUM_CAP`, preferentially killing cells at the group periphery.
 */
struct QuorumCapService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.QUORUM_CAP_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static bool ServiceImpl( Cell& cell, const size_t lev ) {

    const dish2::LogScope guard{ "quorum cap service", "TODO", 3 };

    const size_t num_quorum_bits
      = cell.cell_quorum_state.GetNumKnownQuorumBits( lev );
    if ( num_quorum_bits < cfg.QUORUM_CAP()[ lev ] ) return false;

    const float p = std::min(1.0f,
      cfg.P_QUORUM_CAP_KILL()[ lev ]
      * cell.GetPeripherality( lev )
      //^ don't kill cels on interior, kill periphery
      // more non-kin neighbors = more likely to be killed
      * (num_quorum_bits - cfg.QUORUM_CAP()[ lev ])
      //^ enforce more strictly on large groups
    );
    auto& rand = sgpl::tlrand.Get();
    if ( p && rand.P( p ) ) cell.DeathRoutine(
      dish2::CauseOfDeath::quorum
    );

    return true;

  }


  template<typename Cell>
  static void DoService( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    for (size_t lev{}; lev < spec_t::NLEV; ++lev) {
      if ( ServiceImpl( cell, lev ) ) break;
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_QUORUMCAPSERVICE_HPP_INCLUDE
