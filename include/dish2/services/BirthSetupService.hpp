#pragma once
#ifndef DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE

#include <functional>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/GenomeNodeInputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"
#include "../runninglog/BirthEvent.hpp"

namespace dish2 {

/**
 * Births a new cell into the current cell.
 *
 * This occurs by first iterating through the cell's cardinals' birth request
 * inputs in random order. While the cell's resource stockpile is greater than
 * the `SPAWN_DEFENSE_COST`, the requests are ignored and the stockpile
 * depleted by that cost. The first request that cannot be defended against is
 * then acted upon. The current cell's death routine is called, the old genome
 * is replaced by the incoming genome, and the cell's make-alive routine is
 * called.
 */
struct BirthSetupService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.BIRTH_SETUP_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {
    // TODO check quiescence period?
    const dish2::LogScope guard{ "birth setup service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    thread_local emp::vector< size_t > fresh_input_idxs;
    fresh_input_idxs.clear();

    for (size_t idx{}; idx < cell.cardinals.size(); ++idx) {
      if ( cell.cardinals[idx].genome_node_input.Jump() ) {
        fresh_input_idxs.push_back( idx );
      }
    }

    emp::Shuffle( sgpl::tlrand.Get(), fresh_input_idxs );

    // if current cell has >=1 resource, remove resource before accepting births
    float resource_stockpile = *cell.template begin<
      dish2::ResourceStockpileWrapper<spec_t>
    >();
    while (
      resource_stockpile >= dish2::cfg.SPAWN_DEFENSE_COST()
      && fresh_input_idxs.size()
    ) {
      resource_stockpile -= dish2::cfg.SPAWN_DEFENSE_COST();
      fresh_input_idxs.pop_back();
    }

    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      resource_stockpile
    );

    if ( fresh_input_idxs.size() ) {

      const size_t cardinal_idx = fresh_input_idxs.back();
      const auto& peripheral = cell.cardinals[ cardinal_idx ].peripheral;
      const size_t epoch = peripheral.readable_state.template Get<
        dish2::Epoch
      >();

      const auto& incoming_genome = std::as_const(
        cell.cardinals[ cardinal_idx ].genome_node_input
      ).Get();
      const size_t replev = std::as_const(
        peripheral.state_node_input
      ).Get().template Get< dish2::RepLevRequest< spec_t > >().GetRepLev();

      // record birth event in running log
      const size_t kin_id_commonality_parent_daughter
        = spec_t::NLEV - replev;
      const size_t kin_id_commonality_daughter_eliminated = cell.genome
        ? cell.genome->kin_group_id.CountCommonality(
          incoming_genome.kin_group_id
        )
        : 0;
      const size_t kin_id_commonality_parent_eliminated =
        peripheral.readable_state.template Get<
          dish2::KinGroupIDView<spec_t>
        >().CountCommonality(
          std::as_const( peripheral.state_node_input ).Get().template Get<
            dish2::KinGroupIDView<spec_t>
          >()
        );
      cell.running_logs.Record( dish2::BirthEvent<spec_t>{
        kin_id_commonality_daughter_eliminated,
        kin_id_commonality_parent_daughter,
        kin_id_commonality_parent_eliminated,
        cell.GetPeripherality(),
        replev
      } );

      // setup new genome
      if ( cell.genome.has_value() ) {
        cell.DeathRoutine( dish2::CauseOfDeath::elimination );
      }
      cell.genome = incoming_genome;
      cell.genome->ElapseGeneration( replev, epoch );

      cell.MakeAliveRoutine();

      // record SpawnedFrom direction
      cell.cardinals[cardinal_idx].peripheral.readable_state.template Get<
        dish2::SpawnedFrom
      >() = replev + 1;

    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE
