#pragma once
#ifndef DISH2_RECORD_CELL_CENSUS_WRITE_PERIPHERAL_COUNT_HPP_INCLUDE
#define DISH2_RECORD_CELL_CENSUS_WRITE_PERIPHERAL_COUNT_HPP_INCLUDE

#include <algorithm>

#include "../../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../introspection/any_live_cells.hpp"
#include "../../world/iterators/LiveCellIdxIterator.hpp"
#include "../../world/iterators/LiveCellIterator.hpp"
#include "../../world/iterators/PeripheralityLevWrapper.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void write_peripheral_count(
  const dish2::ThreadWorld<Spec>& world,
  emp::DataFile& file,
  std::string& metric,
  double& value,
  size_t& cell_idx
) {

  const auto& population = world.population;

  using lcit_t = dish2::LiveCellIterator<Spec>;
  using lcidxit_t = dish2::LiveCellIdxIterator<Spec>;

  using iterator_t = dish2::PeripheralityLevWrapper<
    lcit_t
  >;

  for (size_t lev{}; lev < Spec::NLEV; ++lev) {
    metric = emp::to_string("Peripherality Level ", lev);
    if (dish2::any_live_cells<Spec>( world )) uitsl::for_each(
      iterator_t{ lcit_t::make_begin( population ), lev},
      iterator_t{ lcit_t::make_end( population ), lev },
      lcidxit_t::make_begin( population ),
      [&]( const auto& val, const size_t idx ) {
        value = val;
        cell_idx = idx;
        file.Update();
      }
    );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_CELL_CENSUS_WRITE_PERIPHERAL_COUNT_HPP_INCLUDE
