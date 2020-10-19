#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCEDECAYSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_RESOURCEDECAYSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../config/cfg.hpp"

#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ResourceDecayService() {

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

  const float decay_rate = dish2::cfg.RESOURCE_DECAY();

  // update stockpiles to reflect harvested amount
  std::transform(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    [decay_rate](const auto cur) { return cur * decay_rate; }
  );

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCEDECAYSERVICE_HPP_INCLUDE
