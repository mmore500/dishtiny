#pragma once
#ifndef DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE
#define DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/Cell.hpp"
#include "../enum/CauseOfDeath.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void seed_genomes_into(
  const emp::vector< dish2::Genome<Spec> >& seeds,
  dish2::ThreadWorld<Spec>& world
) {

  auto& population = world.population;

  for ( auto& cell : population ) cell.DeathRoutine(
    dish2::CauseOfDeath::birth
  );

  if ( seeds.size() ) {

    // fill incoming_population with genomes from seeds,
    // looping through seeds as necessary to fill incoming_population...
    emp::vector< dish2::Genome<Spec> > incoming_population;
    incoming_population.reserve( population.size() );
    std::transform(
      sgpl::CountingIterator{}, sgpl::CountingIterator{ population.size() },
      std::back_inserter( incoming_population ),
      [&seeds]( const size_t idx ){
        return seeds[ idx % seeds.size() ];
      }
    );

    // shuffle incoming_population...
    emp::Shuffle( sgpl::tlrand.Get(), incoming_population );

    // ... then inject into ThreadWorld's population
    uitsl::for_each(
      std::begin( population ), std::end( population ),
      std::begin( incoming_population ),
      []( auto& cell, const auto& seed ){
        cell.genome = seed;
        cell.genome->SetupSeededGenotype();
        cell.MakeAliveRoutine();
      }
    );

  }

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE
