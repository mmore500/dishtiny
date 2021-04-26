#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_UNIQUE_MODULE_EXPRESSION_PROFILES_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_UNIQUE_MODULE_EXPRESSION_PROFILES_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <set>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/header-only-pca/include/hopca/types.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/summarize_module_expression.hpp"

#include "../world/ThreadWorld.hpp"

#include "count_live_cardinals.hpp"
#include "get_maximum_module_count.hpp"

namespace dish2 {

template< typename Spec >
size_t count_unique_module_expression_profiles(
  const dish2::ThreadWorld< Spec >& world
) {

  const auto& population = world.population;

  using profile_t = emp::vector<size_t>;

  std::set<profile_t> res;

  std::for_each(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    [&]( const auto& cell ){ for (const auto& cardinal : cell) {

      res.insert( sgpl::summarize_module_expression(
        cardinal.cpu,
        cell.genome->program
      ) );

    } }
  );

  return res.size();

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_UNIQUE_MODULE_EXPRESSION_PROFILES_HPP_INCLUDE
