#pragma once
#ifndef DISH2_INTROSPECTION_SUMMARIZE_CAUSES_OF_DEATH_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUMMARIZE_CAUSES_OF_DEATH_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../enum/CauseOfDeath.hpp"
#include "../runninglog/DeathEvent.hpp"

namespace dish2 {

template< typename ThreadWorld >
emp::vector< dish2::CauseOfDeath > summarize_causes_of_death(
  const ThreadWorld& world
) {

  using spec_t = typename ThreadWorld::spec_t;

  const auto& population = world.population;

  emp::vector< dish2::CauseOfDeath > res;

  std::for_each(
    std::begin( population ), std::end( population ),
    [&]( const auto& cell ){
      const auto& log = cell.running_logs.template GetLog<
        dish2::DeathEvent<spec_t>
      >();
      std::transform(
        std::begin( log ), std::end( log ),
        std::back_inserter( res ),
        []( const auto& death_event ){ return death_event.cause_of_death; }
      );
    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUMMARIZE_CAUSES_OF_DEATH_HPP_INCLUDE
