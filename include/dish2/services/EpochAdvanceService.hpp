#pragma once
#ifndef DISH2_SERVICES_EPOCHADVANCESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_EPOCHADVANCESERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/EpochWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborEpochWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * The cell's current-known epoch count is advanced by one then set to the
 * maximum of the cell's current-known epoch count and neighbor cells' current-
 * known epoch count.
 */
struct EpochAdvanceService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.EPOCH_ADVANCE_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "epoch advance service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    const size_t cur_epoch = std::max(
      cell.template begin<dish2::EpochWrapper<spec_t>>()->Get(),
      std::max_element(
        cell.template begin<dish2::NeighborEpochWrapper<spec_t>>(),
        cell.template end<dish2::NeighborEpochWrapper<spec_t>>()
      )->Get()
    );

    // check epoch consistency
    emp_assert((
      std::set<typename dish2::EpochWrapper<spec_t>::value_type>(
        cell.template begin<dish2::EpochWrapper<spec_t>>(),
        cell.template end<dish2::EpochWrapper<spec_t>>()
      ).size() == 1
    ));

    std::fill(
      cell.template begin<dish2::EpochWrapper<spec_t>>(),
      cell.template end<dish2::EpochWrapper<spec_t>>(),
      cur_epoch + 1
    );

    // check epoch consistency
    emp_assert((
      std::set<typename dish2::EpochWrapper<spec_t>::value_type>(
        cell.template begin<dish2::EpochWrapper<spec_t>>(),
        cell.template end<dish2::EpochWrapper<spec_t>>()
      ).size() == 1
    ));

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_EPOCHADVANCESERVICE_HPP_INCLUDE
