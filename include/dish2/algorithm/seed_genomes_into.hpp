#pragma once
#ifndef DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE
#define DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void seed_genomes_into(
  const emp::vector< dish2::Genome<Spec> >& seeds,
  dish2::ThreadWorld<Spec>& world
) {

  auto& population = world.population;

  for ( auto& cell : population ) cell.DeathRoutine();

  if ( seeds.size() ) uitsl::for_each(
    std::begin( population ),
    std::end( population ),
    sgpl::CountingIterator{},
    [&seeds]( auto& cell, const size_t idx ){
      cell.genome = seeds[ idx % seeds.size() ];
      cell.genome->SetupSeededGenotype();
      cell.MakeAliveRoutine();
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE
