#pragma once
#ifndef DISH2_SERVICES_THREADLOCAL_DIVERSITYMAINTENANCESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_THREADLOCAL_DIVERSITYMAINTENANCESERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../enum/CauseOfDeath.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_root_id_counts.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"

namespace dish2 {

/**
 * Prevents any one originally-generated ancestor from sweeping the population,
 * preserving deep phylogenetic diversity.
 *
 * Counts cells that descend from each originally-generated ancestor. If more
 * than `DIVERSITY_MAINTENANCE_PREVALENCE` of cells descend from a single
 * originally-generated ancestor, decay their resource stockpiles. The
 * magnitude of this effect increases with excess prevalence.
 */
struct DiversityMaintenanceService {

  static bool ShouldRun( const size_t update ) {
    const size_t freq = dish2::cfg.DIVERSITY_MAINTENANCE_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0
      && dish2::cfg.GENESIS() != "innoculate"
      && dish2::cfg.GENESIS() != "monoculture"
    ;
  }

  template<typename ThreadWorld>
  static void DoService( ThreadWorld& thread_world, const size_t ) {

    using spec_t = typename ThreadWorld::spec_t;

    const dish2::LogScope guard{ "diversity maintentance service", "TODO", 3 };

    auto& population = thread_world.population;

    const auto root_id_counts = dish2::get_root_id_counts( thread_world );

    const size_t threshold_count = (
      dish2::cfg.DIVERSITY_MAINTENANCE_PREVALENCE()
      * static_cast<double>( dish2::count_live_cells( thread_world ) )
    );

    const auto begin = dish2::LiveCellIterator<spec_t>::make_begin(population);
    const auto end = dish2::LiveCellIterator<spec_t>::make_end(population);

    uitsl::for_each(
      begin, end,
      dish2::RootIDValWrapper{ begin },
      [&root_id_counts, threshold_count]( auto& cell, const size_t root_id ){
        const size_t count = root_id_counts.at( root_id );

        if ( threshold_count && count > threshold_count ) {
          const size_t excess = count - threshold_count;

          const double excess_frac
            = excess / static_cast<double>( threshold_count );
          emp_assert( excess_frac >= 0, excess_frac );
          emp_assert( std::isfinite(excess_frac), excess_frac );

          const double decay = std::max( 1.0 - excess_frac, 0.0 );

          emp_assert( std::isfinite(decay), decay );

          // update stockpiles to reflect decay
          std::transform(
            cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
            cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
            cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
            [decay](const auto cur) { return cur * decay; }
          );

          emp_assert( std::none_of(
            cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
            cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
            []( const auto val ){ return std::isnan( val ); }
          ), decay );
          emp_assert( std::none_of(
            cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
            cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
            []( const auto val ){ return std::isinf( val ); }
          ), decay );

        }

      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_THREADLOCAL_DIVERSITYMAINTENANCESERVICE_HPP_INCLUDE
