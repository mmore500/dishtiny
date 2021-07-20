#pragma once
#ifndef DISH2_SERVICES_CELLAGESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CELLAGESERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/CellAgeWrapper.hpp"
#include "../cell/cardinal_iterators/EpochWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Advances the cell age introspective state and refreshes kin group age
 * introspective state.
 *
 * @todo Refactor out GroupAgeService.
 */
struct CellAgeService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.CELL_AGE_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    const dish2::LogScope guard{ "cell age service", "TODO", 3 };

    // check cell age consistency
    emp_assert((
      std::set<typename dish2::CellAgeWrapper<spec_t>::value_type>(
        cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
        cell.template end<dish2::CellAgeWrapper<spec_t>>()
      ).size() == 1
    ));

    std::transform(
      cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
      cell.template end<dish2::CellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
      [](const auto cell_age){ return cell_age + 1; }
    );

    // check cell age consistency
    emp_assert((
      std::set<typename dish2::CellAgeWrapper<spec_t>::value_type>(
        cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
        cell.template end<dish2::CellAgeWrapper<spec_t>>()
      ).size() == 1
    ));

    const auto& kin_group_epoch_stamps = cell.genome->kin_group_epoch_stamps;
    const size_t epoch = *cell.template begin<dish2::EpochWrapper<spec_t>>();
    std::for_each(
      cell.template begin<dish2::KinGroupAgeWrapper<spec_t>>(),
      cell.template end<dish2::KinGroupAgeWrapper<spec_t>>(),
      [&kin_group_epoch_stamps, epoch]( auto& kin_group_age ){
        kin_group_age.Refresh( epoch, kin_group_epoch_stamps );
      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CELLAGESERVICE_HPP_INCLUDE
