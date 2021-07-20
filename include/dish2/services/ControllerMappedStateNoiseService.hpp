#pragma once
#ifndef DISH2_SERVICES_CONTROLLERMAPPEDSTATENOISESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CONTROLLERMAPPEDSTATENOISESERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/mutate_bits.hpp"

#include "../cell/cardinal_iterators/ControllerMappedStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Introduce noise to cardinals' outputs.
 *
 * Picks a random number $n$ from a Poisson distribution parameterized by
 * `CONTROLLER_MAPPED_STATE_DEFECT_RATE`. Then, it introduces $n$ defects to a
 * cell's controller-mapped writable state. Half of these defects zero out the
 * state and half randomize it.
 */
struct ControllerMappedStateNoiseService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.CONTROLLER_MAPPED_STATE_NOISE_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{
      "controller mapped state noise service", "TODO", 3
    };

    using spec_t = typename Cell::spec_t;

    const double p_defect = dish2::cfg.CONTROLLER_MAPPED_STATE_DEFECT_RATE();

    // bail early for p_defect == 0
    if ( p_defect == 0 ) return;

    std::for_each(
      cell.template begin<dish2::ControllerMappedStateWrapper<spec_t>>(),
      cell.template end<dish2::ControllerMappedStateWrapper<spec_t>>(),
      [p_defect]( auto& controller_mapped_state ){

        auto& rng = sgpl::tlrand.Get();

        // ideally, we would draw from the binomial distn,
        // but that's expensive with varying n...
        // so approximate with the poisson distribution instead
        // they're similar-ish, e.g., https://www.researchgate.net/figure/Poisson-versus-binomial-distribution-from-number-of-heads-in-a-coin-toss-The-Poisson_fig3_255717571
        // (they become more similar for large n)
        const size_t n_defects = rng.GetRandPoisson(
          dish2::ControllerMappedState<spec_t>::parent_size,
          p_defect
        );

        for ( size_t i{}; i < n_defects; ++i ) {
          const size_t target = rng.GetUInt(
            dish2::ControllerMappedState<spec_t>::parent_size
          );
          if ( rng.P( 0.5 ) ) {
            controller_mapped_state.Assign( target, 0.0f );
          } else controller_mapped_state.Randomize( target );
        }

      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CONTROLLERMAPPEDSTATENOISESERVICE_HPP_INCLUDE
