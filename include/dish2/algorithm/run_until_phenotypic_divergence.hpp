#pragma once
#ifndef DISH2_ALGORITHM_RUN_UNTIL_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
#define DISH2_ALGORITHM_RUN_UNTIL_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/TemporaryThreadIdxOverride.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../introspection/compare_resource_stockpiles.hpp"
#include "../world/ProcWorld.hpp"
#include "../world/ThreadWorld.hpp"

#include "seed_genomes_into.hpp"

namespace dish2 {

template< typename Spec >
size_t run_until_phenotypic_divergence(
  dish2::ThreadWorld<Spec>& control,
  dish2::ThreadWorld<Spec>& experiment
) {

  for ( size_t upd{}; upd < cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES(); ++upd ) {

    emp_assert(
      dish2::compare_resource_stockpiles<Spec>( control, experiment )
    );

    const emp::Random bak = sgpl::tlrand.Get();

    control.Update();

    sgpl::tlrand.Get() = bak;

    experiment.template Update<false>();

    if ( !dish2::compare_resource_stockpiles<Spec>( control, experiment ) ) {
      dish2::log_event( {
        dish2::result_fail,
        emp::to_string( "phenotypic divergence detected at update ", upd )
      } );
      return upd;
    }

    if ( upd % 100 == 0 ) dish2::log_event( {
      dish2::info, emp::to_string( "update ", upd )
    } );

  }

  dish2::log_event( {
    dish2::result_success,
    emp::to_string(
      "no divergence detected after ",
      cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES(),
      " updates"
    )
  } );

  return cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES();

}

template< typename Spec >
size_t run_until_phenotypic_divergence(
  const dish2::Genome<Spec>& genome1,
  const dish2::Genome<Spec>& genome2,
  const emp::vector<dish2::Genome<Spec>>& background_population={}
) {

  using vec_genome_t =  emp::vector<dish2::Genome<Spec>>;
  using vec_vec_genome_t =  emp::vector<vec_genome_t>;

  // if genomes are equivalent, no need to test
  if ( genome1 == genome2 ) return cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES();

  netuit::internal::MeshIDCounter::Reset();

  dish2::TemporaryThreadIdxOverride override{ 0 };

  const emp::Random rng_bak = sgpl::tlrand.Get();

  // set consistent rng state
  sgpl::tlrand.Get() = emp::Random(1);

  auto world1 = dish2::ProcWorld<Spec>{}.MakeThreadWorld();
  dish2::seed_genomes_into<Spec>(
    background_population.size()
      ? vec_vec_genome_t{vec_genome_t{genome1}, background_population}
      : vec_vec_genome_t{vec_genome_t{genome1}},
    world1
  );

  // roll back rng state
  sgpl::tlrand.Get() = emp::Random(1);

  auto world2 = dish2::ProcWorld<Spec>{}.MakeThreadWorld();
  dish2::seed_genomes_into<Spec>(
    background_population.size()
      ? vec_vec_genome_t{vec_genome_t{genome2}, background_population}
      : vec_vec_genome_t{vec_genome_t{genome2}},
    world2
  );

  sgpl::tlrand.Get() = emp::Random(1);

  const auto res = run_until_phenotypic_divergence<Spec>( world1, world2 );

  // restore RNG state
  sgpl::tlrand.Get() = rng_bak;

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_RUN_UNTIL_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
