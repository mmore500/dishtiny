#pragma once
#ifndef DISH2_CELL_SERVICES_RESOURCEDECAYSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICES_RESOURCEDECAYSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

struct ResourceDecayService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_DECAY_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<Spec>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
        cell.template end<dish2::ResourceStockpileWrapper<Spec>>()
      ).size() == 1
    ));

    const float decay_rate = dish2::cfg.RESOURCE_DECAY();

    // update stockpiles to reflect harvested amount
    std::transform(
      cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
      cell.template end<dish2::ResourceStockpileWrapper<Spec>>(),
      cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
      [decay_rate](const auto cur) { return cur * decay_rate; }
    );

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<Spec>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
        cell.template end<dish2::ResourceStockpileWrapper<Spec>>()
      ).size() == 1
    ));

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICES_RESOURCEDECAYSERVICE_HPP_INCLUDE
