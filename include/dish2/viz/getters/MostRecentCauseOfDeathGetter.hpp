#pragma once
#ifndef DISH2_VIZ_GETTERS_MOSTRECENTCAUSEOFDEATHGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_MOSTRECENTCAUSEOFDEATHGETTER_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../runninglog/DeathEvent.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class MostRecentCauseOfDeathGetter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

public:

  using value_type = emp::optional<dish2::CauseOfDeath>;

  template< typename... Args >
  MostRecentCauseOfDeathGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  )
  : thread_world(thread_world_)
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {

    const auto& cell = thread_world.get().GetCell( cell_idx );
    const auto& death_log = cell.running_logs.template GetLog<
      dish2::DeathEvent<Spec>
    >();

    if ( death_log.IsEmpty() ) return std::nullopt;
    else return { death_log.GetMostRecent().cause_of_death };

  }

  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_MOSTRECENTCAUSEOFDEATHGETTER_HPP_INCLUDE
