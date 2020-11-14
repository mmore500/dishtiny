#pragma once
#ifndef DISH2_INTROSPECTION_GET_MAXIMUM_MODULE_COUNT_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MAXIMUM_MODULE_COUNT_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
size_t get_maximum_module_count( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  emp::vector< size_t > module_counts;
  std::transform(
    ++dish2::LiveCellIterator<Spec>(
      std::prev( std::begin( population ) ), std::end( population )
    ),
    dish2::LiveCellIterator<Spec>( std::end(population), std::end(population) ),
    std::back_inserter( module_counts ),
    []( const dish2::Cell<Spec>& cell ){
      return sgpl::count_modules( cell.genome->program );
    }
  );

  return *std::max_element(
    std::begin( module_counts ),
    std::end( module_counts )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MAXIMUM_MODULE_COUNT_HPP_INCLUDE
