#pragma once
#ifndef DISH2_SERVICES_DECAYTOBASELINESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_DECAYTOBASELINESERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/ControllerMappedStateWrapper.hpp"
#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../cell/cardinal_iterators/TransientNopStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Decays a cell's global regulators, resets its controller-mapped peripheral
 * states, and resets its transient NOP states.
 */
struct DecayToBaselineService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.DECAY_TO_BASELINE_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "decay to baseline service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // decay regulators
    std::for_each(
      cell.template begin< dish2::CpuWrapper<spec_t> >(),
      cell.template end< dish2::CpuWrapper<spec_t> >(),
      []( auto& cpu ) { cpu.DecayGlobalRegulators(); }
    );

    // reset controller-mapped cpu peripheral state
    std::for_each(
      cell.template begin< dish2::ControllerMappedStateWrapper<spec_t> >(),
      cell.template end< dish2::ControllerMappedStateWrapper<spec_t> >(),
      []( auto& state ) { state.Reset(); }
    );

    // reset transient nop cpu peripheral state
    std::for_each(
      cell.template begin< dish2::TransientNopStateWrapper<spec_t> >(),
      cell.template end< dish2::TransientNopStateWrapper<spec_t> >(),
      []( auto& state ) { state.Reset(); }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_DECAYTOBASELINESERVICE_HPP_INCLUDE
