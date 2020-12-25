#pragma once
#ifndef DISH2_RECORD_CELL_CENSUS_WRITE_SPAWN_COUNT_HPP_INCLUDE
#define DISH2_RECORD_CELL_CENSUS_WRITE_SPAWN_COUNT_HPP_INCLUDE

#include <algorithm>

#include "../../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../../cell/cardinal_iterators/SpawnCountWrapper.hpp"
#include "../../world/iterators/LiveCellIterator.hpp"
#include "../../world/iterators/WorldIteratorAbridger.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void write_spawn_count(
  const dish2::ThreadWorld<Spec>& world,
  emp::DataFile& file,
  std::string& metric,
  double& value,
  size_t& cell_idx
) {

  metric = "Spawn Count";

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;

  using iterator_t = dish2::WorldIteratorAbridger<
    lcit_t,
    dish2::SpawnCountWrapper<Spec>
  >;

  uitsl::for_each(
    iterator_t{ lcit_t::make_begin( population ) },
    iterator_t{ lcit_t::make_end( population ) },
    sgpl::CountingIterator{},
    [&]( const auto& val, const size_t idx ) {
      value = val;
      cell_idx = idx;
      file.Update();
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_CELL_CENSUS_WRITE_SPAWN_COUNT_HPP_INCLUDE
