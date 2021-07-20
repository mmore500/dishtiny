#pragma once
#ifndef DISH2_SERVICES_THREADLOCAL_CELLUPDATESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_THREADLOCAL_CELLUPDATESERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../enum/CauseOfDeath.hpp"
#include "../utility/IndexShuffler.hpp"

namespace dish2 {

/**
 * Performs each cell's simulation services, iterating over cells in randomized
 * order.
 */
struct CellUpdateService {

  static bool ShouldRun( const size_t ) { return true; }

  template<typename ThreadWorld>
  static void DoService( ThreadWorld& thread_world, const size_t update ) {

    const dish2::LogScope guard{ "cell update service", "TODO", 3 };

    auto& population = thread_world.population;

    // this could be thread_local,
    // but then we'd have to be careful about ensuring that holdover state
    // doesn't affect phenotypic divergence tests
    dish2::IndexShuffler shuffler( population.size() );
    shuffler.Shuffle();

    for ( const size_t i : shuffler ) {
      const dish2::LogScope guard{
        emp::to_string("updating cell ", i),
        "We're having the nth cell run its program and interact with the environment. All cells will take a turn at this one-by-one.",
        3
      };
      auto& cell = population[i];
      cell.Update(update);
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_THREADLOCAL_CELLUPDATESERVICE_HPP_INCLUDE
