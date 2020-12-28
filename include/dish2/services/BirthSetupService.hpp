#pragma once
#ifndef DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE

#include <functional>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"
#include "../cell/cardinal_iterators/GenomeNodeInputWrapper.hpp"
#include "../debug/LogScope.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

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

    if ( fresh_input_idxs.size() ) {

      const size_t cardinal_idx = fresh_input_idxs[
        sgpl::tlrand.Get().GetUInt( fresh_input_idxs.size() )
      ];
      const size_t epoch = cell.cardinals[
        cardinal_idx
      ].peripheral.readable_state.template Get<
        dish2::Epoch
      >();

      cell.DeathRoutine();

      cell.genome = cell.cardinals[cardinal_idx].genome_node_input.Get();

      const auto& replev_request = cell.cardinals[
        cardinal_idx
      ].peripheral.state_node_input.Get().template Get<
        dish2::RepLevRequest< spec_t >
      >();
      cell.genome->ElapseGeneration( replev_request.GetRepLev(), epoch );

      cell.MakeAliveRoutine();
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE
