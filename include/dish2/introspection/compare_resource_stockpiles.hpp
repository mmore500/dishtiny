#pragma once
#ifndef DISH2_INTROSPECTION_COMPARE_RESOURCE_STOCKPILES_HPP_INCLUDE
#define DISH2_INTROSPECTION_COMPARE_RESOURCE_STOCKPILES_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../cell/Cell.hpp"
#include "../world/iterators/WorldIteratorAbridger.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
bool compare_resource_stockpiles(
  const dish2::ThreadWorld<Spec>& world1,
  const dish2::ThreadWorld<Spec>& world2
) {

  const auto& population1 = world1.population;
  const auto& population2 = world2.population;

  using iterator_t = dish2::WorldIteratorAbridger<
    typename emp::vector< dish2::Cell< Spec > >::const_iterator,
    dish2::ResourceStockpileWrapper<Spec>
  >;

  return std::equal(
    iterator_t{ std::begin(population1) },
    iterator_t{ std::end(population1) },
    iterator_t{ std::begin(population2) }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COMPARE_RESOURCE_STOCKPILES_HPP_INCLUDE
