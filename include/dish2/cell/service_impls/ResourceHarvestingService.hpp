#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ResourceHarvestingService() {

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

  // how much resource have we harvested
  // TODO paramaterize and base on the number of cpu cycles elapsed
  const float harvest = 0.005;

  // update stockpiles to reflect harvested amount
  std::transform(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    [harvest](const auto cur) { return cur + harvest; }
  );

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCEHARVESTINGSERVICE_HPP_INCLUDE
