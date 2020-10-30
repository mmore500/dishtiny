#pragma once
#ifndef DISH2_CELL_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../../config/cfg.hpp"

#include "../cardinal_iterators/ResourceNodeInputWrapper.hpp"
#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

struct ResourceReceivingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_RECEIVING_SERVICE_FREQUENCY();
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

    // how much resource have we received across all cardinals?
    const float received_amount = std::accumulate(
      cell.template begin<dish2::ResourceNodeInputWrapper<Spec>>(),
      cell.template end<dish2::ResourceNodeInputWrapper<Spec>>(),
      0.0f,
      [](const auto& cumulative_sum, auto& addend){ return addend.JumpGet(); }
    );

    // how much do we already have?
    const float current_amount
      = *cell.template begin<dish2::ResourceStockpileWrapper<Spec>>();

    // update stockpiles
    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
      cell.template end<dish2::ResourceStockpileWrapper<Spec>>(),
      received_amount + current_amount
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

#endif // #ifndef DISH2_CELL_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
