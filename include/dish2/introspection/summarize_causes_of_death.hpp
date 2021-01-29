#pragma once
#ifndef DISH2_INTROSPECTION_SUMMARIZE_CAUSES_OF_DEATH_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUMMARIZE_CAUSES_OF_DEATH_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../enum/CauseOfDeath.hpp"

namespace dish2 {

template< typename ThreadWorld >
emp::vector< dish2::CauseOfDeath > summarize_causes_of_death(
  const ThreadWorld& world
) {

  const auto& population = world.population;

  emp::vector< dish2::CauseOfDeath > res;

  std::for_each(
    std::begin( population ),
    std::end( population ),
    [&]( const auto& cell ){ res.insert(
      std::end( res ), std::begin( cell.death_log ), std::end( cell.death_log )
    ); }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUMMARIZE_CAUSES_OF_DEATH_HPP_INCLUDE
