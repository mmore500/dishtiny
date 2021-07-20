#pragma once
#ifndef DISH2_SERVICES_COLLECTIVEHARVESTINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_COLLECTIVEHARVESTINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Perform kin group collective resource harvest for single cell.
 *
 * Calculates the total amount of resource collectively harvested to this cell
 * by the cell's kin group. This amount increases with quorum count and
 * saturates at `OPTIMAL_QUORUM_COUNT`. Adds the harvested amount to the cell's
 * resource stockpile.
 */
class CollectiveHarvestingService {

  template<typename Cell>
  static float CalcHarvest( const Cell& cell, const size_t lev ) {

    const size_t optimum = cfg.OPTIMAL_QUORUM_COUNT()[lev];
    const size_t quorum_count = cell.cell_quorum_state.GetNumKnownQuorumBits(
      lev
    );
    emp_assert( quorum_count );
    const size_t effective_count = std::min(
      optimum,
      // subtract 1 from quorum count for self, being cautious about overflow
      // TODO only subtract if self has a quorum bit set
      quorum_count - static_cast<bool>( quorum_count )
    );

    return cfg.BASE_HARVEST_RATE() * effective_count;
  }

public:

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.COLLECTIVE_HARVESTING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "collective harvesting service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check resource stockpile consistency
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    // how much resource have we harvested?
    float harvest{};
    for (size_t lev{}; lev < spec_t::NLEV; ++lev) {
      harvest += CalcHarvest<Cell>( cell, lev );
    }

    emp_assert( std::isfinite( harvest ), harvest );

    // update stockpiles to reflect harvested amount
    std::transform(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      [harvest](const auto cur) { return cur + harvest; }
    );

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isnan( val ); }
    ), harvest );



  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_COLLECTIVEHARVESTINGSERVICE_HPP_INCLUDE
