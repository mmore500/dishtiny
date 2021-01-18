#pragma once
#ifndef DISH2_SERVICES_WRITABLESTATENOISESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_WRITABLESTATENOISESERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/mutate_bits.hpp"

#include "../cell/cardinal_iterators/WritableStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

struct WritableStateNoiseService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.WRITABLE_STATE_NOISE_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "writable state noise service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    const double p_bit_toggle = dish2::cfg.WRITABLE_STATE_BIT_DEFECT_RATE();

    // bail early for p_bit_toggle == 0
    if ( p_bit_toggle == 0 ) return;

    std::for_each(
      cell.template begin<dish2::WritableStateWrapper<spec_t>>(),
      cell.template end<dish2::WritableStateWrapper<spec_t>>(),
      [p_bit_toggle]( auto& writable_state ){

        // ideally, we would draw from the binomial distn,
        // but that's expensive with varying n...
        // so approximate with the poisson distribution instead
        // they're similar-ish, e.g., https://www.researchgate.net/figure/Poisson-versus-binomial-distribution-from-number-of-heads-in-a-coin-toss-The-Poisson_fig3_255717571
        // (they become more similar for large n)
        const size_t n_muts = sgpl::tlrand.Get().GetRandPoisson(
          sizeof(dish2::WritableState< spec_t >) * CHAR_BIT,
          p_bit_toggle
        );

        sgpl::mutate_bits(
          std::span< std::byte >{
            reinterpret_cast<std::byte*>( &writable_state ),
            sizeof( writable_state )
          },
          n_muts
        );

      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_WRITABLESTATENOISESERVICE_HPP_INCLUDE
