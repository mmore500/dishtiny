#pragma once
#ifndef DISH2_ALGORITHM_DETECT_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
#define DISH2_ALGORITHM_DETECT_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../introspection/compare_resource_stockpiles.hpp"
#include "../world/ThreadWorld.hpp"

#include "seed_genomes_into.hpp"

namespace dish2 {

template< typename Spec >
bool detect_phenotypic_divergence(
  dish2::ThreadWorld<Spec>& world1,
  dish2::ThreadWorld<Spec>& world2
) {

  for ( size_t upd{}; upd < cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES(); ++upd ) {

    const emp::Random bak = sgpl::ThreadLocalRandom::Get();

    world1.Update();

    sgpl::ThreadLocalRandom::Get() = bak;

    world2.Update();

    if ( !dish2::compare_resource_stockpiles<Spec>( world1, world2 ) ) {
      dish2::log_event( {
        dish2::result_fail,
        emp::to_string( "phenotypic divergence detected at update ", upd )
      } );
      return true;
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

  return false;

}

template< typename Spec >
bool detect_phenotypic_divergence(
  const dish2::Genome<Spec>& genome1,
  const dish2::Genome<Spec> genome2
) {


  netuit::internal::MeshIDCounter::Reset();

  // store old N_CELLS value, temporarily overwrite it
  const auto n_cells_bak = cfg.N_CELLS();
  cfg.Set( "N_CELLS", emp::to_string(cfg.PHENOTYPIC_DIVERGENCE_N_CELLS()) );
  const auto n_threads_bak = cfg.N_THREADS();
  cfg.Set( "N_THREADS", "1" );

  const emp::Random rng_bak = sgpl::ThreadLocalRandom::Get();

  auto world1 = dish2::ProcWorld<Spec>{}.MakeThreadWorld(0);
  dish2::seed_genomes_into<Spec>( {genome1}, world1 );

  // roll back rng state
  sgpl::ThreadLocalRandom::Get() = rng_bak;

  auto world2 = dish2::ProcWorld<Spec>{}.MakeThreadWorld(0);
  dish2::seed_genomes_into<Spec>( {genome2}, world2 );

  // restore N_CELLS value
  cfg.Set( "N_CELLS", emp::to_string(n_cells_bak) );
  cfg.Set( "N_THREADS", emp::to_string(n_threads_bak) );

  return detect_phenotypic_divergence<Spec>( world1, world2 );

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_DETECT_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
