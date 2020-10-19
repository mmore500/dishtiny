#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../config/cfg.hpp"

#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ResourceHarvestingService() {

  // check resource stockpile consistency
  emp_assert((
    std::set< typename dish2::ResourceStockpileWrapper<Spec>::value_type >(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
    ).size() == 1
  ));

  // how much resource have we harvested
  const float harvest = dish2::cfg.HARVEST_RATE();

  // update stockpiles to reflect harvested amount
  std::transform(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    [harvest](const auto cur) { return cur + harvest; }
  );

  // check resource stockpile consistency
  emp_assert((
    std::set<typename dish2::ResourceStockpileWrapper<Spec>::value_type >(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
    ).size() == 1
  ));

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE
