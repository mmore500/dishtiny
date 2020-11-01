#pragma once
#ifndef DISH2_SERVICES_CELLAGESERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CELLAGESERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../config/cfg.hpp"
#include "../cell/cardinal_iterators/CellAgeWrapper.hpp"

namespace dish2 {

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

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CELLAGESERVICE_HPP_INCLUDE
