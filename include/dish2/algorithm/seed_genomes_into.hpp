#pragma once
#ifndef DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE
#define DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/Cell.hpp"
#include "../config/cfg.hpp"
#include "../enum/CauseOfDeath.hpp"
#include "../record/StintRootIDChangeoverRecorder.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec, bool record_stint_root_id_changeover=false >
void seed_genomes_into(
  emp::vector< emp::vector<dish2::Genome<Spec>> > seed_buckets,
  dish2::ThreadWorld<Spec>& world
) {

  auto& population = world.population;

  for ( auto& cell : population ) cell.DeathRoutine(
    dish2::CauseOfDeath::elimination
  );

  if ( seed_buckets.size() ) {

    // shuffle each bucket
    for ( auto& bucket : seed_buckets ) {
      emp::Shuffle( sgpl::tlrand.Get(), bucket );
    }

    const size_t fill_to = population.size() * cfg.SEED_FILL_FRACTION();

    // fill incoming_population with genomes from seed buckets,
    // looping through buckets as necessary to fill incoming_population...
    emp::vector< dish2::Genome<Spec> > incoming_population;
    incoming_population.reserve( fill_to );
    std::transform(
      sgpl::CountingIterator{}, sgpl::CountingIterator{ fill_to },
      std::back_inserter( incoming_population ),
      [&seed_buckets]( const size_t idx ){
        const size_t bucket_idx = idx % seed_buckets.size();
        const size_t bucket_pass_count = idx / seed_buckets.size();
        const size_t cur_bucket_length = seed_buckets[bucket_idx].size();
        return seed_buckets[
          bucket_idx
        ][
          bucket_pass_count % cur_bucket_length
        ];
      }
    );

    // shuffle incoming_population...
    emp::Shuffle( sgpl::tlrand.Get(), incoming_population );

    emp::optional<dish2::StintRootIDChangeoverRecorder> recorder;
    if constexpr ( record_stint_root_id_changeover ) recorder.emplace();

    // ... then inject into ThreadWorld's population
    uitsl::for_each(
      std::begin( incoming_population ), std::end( incoming_population ),
      std::begin( population ),
      [&recorder]( const auto& seed, auto& cell ){
        cell.genome = seed;
        cell.genome->SetupSeededGenotype();
        cell.MakeAliveRoutine();

        if constexpr (
          record_stint_root_id_changeover
        ) recorder->record_changeover(
          seed.root_id.GetID(),
          seed.stint_root_id.GetID(),
          cell.genome->stint_root_id.GetID()
        );
        // suppress unused capture compiler warning
        // adapted from https://stackoverflow.com/a/65595121
        else (void)recorder;

      }
    );

  }

}

template< typename Spec, bool record_stint_root_id_changeover=false >
void seed_genomes_into(
  const emp::vector< dish2::Genome<Spec> >& seeds,
  dish2::ThreadWorld<Spec>& world
) {

  emp::vector< emp::vector<dish2::Genome<Spec>> > seed_buckets;

  std::transform(
    std::begin( seeds ), std::end( seeds ),
    std::back_inserter( seed_buckets ),
    []( const auto& seed ){ return emp::vector<dish2::Genome<Spec>>{ seed }; }
  );

  dish2::seed_genomes_into<Spec, record_stint_root_id_changeover>(
    seed_buckets, world
  );

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_SEED_GENOMES_INTO_HPP_INCLUDE
