#pragma once
#ifndef DISH2_SERVICES_INTERMITTENTINTROSPECTIVESTATEROTATERESTORESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMITTENTINTROSPECTIVESTATEROTATERESTORESERVICE_HPP_INCLUDE

#include <algorithm>
#include <numeric>

#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/algorithm/execute_cpu.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/IntrospectiveStateIndexedSwapWrapper.hpp"
#include "../cell/cardinal_iterators/IntrospectiveStateWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

#include "IntermittentIntrospectiveStateRotateService.hpp"

namespace dish2 {

struct IntermittentIntrospectiveStateRotateRestoreService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq
      = dish2::cfg.INTERMITTENT_STATE_PERTURB_SERVICES_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{
      "intermittent introspective state rotate restore service", "TODO", 3
    };

    using spec_t = typename Cell::spec_t;

    const size_t rotation = std::exchange(
      IntermittentIntrospectiveStateRotateService::current_rotation,
      0
    );

    if ( rotation == 0 ) return;

    const auto& perturbation_config = cell.genome->GetRootPerturbationConfig();
    const auto& target_idx = perturbation_config.introspective_state_target_idx;

    using indexed_swapper_t = dish2::IntrospectiveStateIndexedSwapWrapper<spec_t>;

    if ( target_idx.has_value() ) std::rotate(
      cell.template begin<indexed_swapper_t>( *target_idx ),
      cell.template begin<indexed_swapper_t>( *target_idx ) + rotation,
      cell.template end<indexed_swapper_t>()
    ); else std::rotate(
      cell.template begin<dish2::IntrospectiveStateWrapper<spec_t>>(),
      cell.template begin<dish2::IntrospectiveStateWrapper<spec_t>>()
        + cell.GetNumCardinals() - rotation,
      cell.template end<dish2::IntrospectiveStateWrapper<spec_t>>()
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMITTENTINTROSPECTIVESTATEROTATERESTORESERVICE_HPP_INCLUDE
