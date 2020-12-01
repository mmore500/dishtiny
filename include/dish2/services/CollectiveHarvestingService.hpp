#pragma once
#ifndef DISH2_SERVICES_COLLECTIVEHARVESTINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_COLLECTIVEHARVESTINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

class CollectiveHarvestingService {

  template<typename Cell>
  static float CalcHarvest( const Cell& cell, const size_t lev ) {

    using spec_t = typename Cell::spec_t;

    const size_t group_age
      = cell.template begin<dish2::KinGroupAgeWrapper<spec_t>>()->GetBuffer(
      )[ lev ];

    const bool is_group_expired
      = group_age > cfg.GROUP_EXPIRATION_DURATIONS()[ lev ];

    if ( is_group_expired ) return 0.0f;

    const size_t optimum = cfg.OPTIMAL_QUORUM_COUNT()[lev];
    const size_t quorum_count = cell.cell_quorum_state.GetNumKnownQuorumBits(
      lev
    );
    const size_t distance_from_optimum
      = std::max(optimum, quorum_count) - std::min(optimum, quorum_count);
    const float resource_penalty_rate
      = 1.0f / cfg.OPTIMAL_QUORUM_COUNT()[lev];
    const float resource_penalty
      = distance_from_optimum * resource_penalty_rate;
    return std::max(
      cfg.COLLECTIVE_HARVEST_RATE()[lev] - resource_penalty,
      0.0f
    );
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
    for (size_t lev{}; lev < Spec::NLEV; ++lev) {
      harvest += CalcHarvest<Cell>( cell, lev );
    }

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

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_COLLECTIVEHARVESTINGSERVICE_HPP_INCLUDE
