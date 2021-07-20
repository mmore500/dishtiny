#pragma once
#ifndef DISH2_SERVICES_COLLECTIVERESOURCEDECAYSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_COLLECTIVERESOURCEDECAYSERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Prevent excessive group growth.
 *
 * If the cell's quorum count exceeds `OPTIMAL_QUORUM_COUNT`, applies
 * multiplicative decay to the cell's resource stockpile. This effect
 * strengthens exponentially with excess cell quorum count.
 */
class CollectiveResourceDecayService {

  template<typename Cell>
  static float CalcDecayRate( const Cell& cell, const size_t lev ) {

    const size_t optimum = cfg.OPTIMAL_QUORUM_COUNT()[lev];
    const size_t quorum_count = cell.cell_quorum_state.GetNumKnownQuorumBits(
      lev
    );

    if ( quorum_count > optimum ) {
      const size_t num_over = quorum_count - optimum;
      const float res = std::pow(
        dish2::cfg.RESOURCE_DECAY(),
        std::pow( 2, num_over )
      );
      emp_assert( std::isfinite( res ), res, num_over );
      return res;
    } else return 1.0f;

  }

public:

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_DECAY_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "collective resource decay service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    float decay_rate = 1.0f;
    for (size_t lev{}; lev < spec_t::NLEV; ++lev) {
      decay_rate *= CalcDecayRate( cell, lev );
    }

    emp_assert( std::isfinite( decay_rate ), decay_rate );

    // update stockpiles to reflect decay
    std::transform(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      [decay_rate](const auto cur) { return cur * decay_rate; }
    );

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isnan( val ); }
    ), decay_rate );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_COLLECTIVERESOURCEDECAYSERVICE_HPP_INCLUDE
