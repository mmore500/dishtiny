#pragma once
#ifndef DISH2_SERVICES_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Adds a constant amount to cell's resource stockpile.
 */
struct ResourceHarvestingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_HARVESTING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "resource harvesting service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check resource stockpile consistency
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    // how much resource have we harvested
    const float harvest = dish2::cfg.BASE_HARVEST_RATE();

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

#endif // #ifndef DISH2_SERVICES_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE
