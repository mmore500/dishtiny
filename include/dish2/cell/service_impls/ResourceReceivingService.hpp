#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_RESOURCERECEIVINGSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../cardinal_iterators/ResourceNodeInputWrapper.hpp"
#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ResourceReceivingService() {

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

  // how much resource have we received across all cardinals?
  const float received_amount = std::accumulate(
    begin<dish2::ResourceNodeInputWrapper<Spec>>(),
    end<dish2::ResourceNodeInputWrapper<Spec>>(),
    0.0f,
    [](const auto& cumulative_sum, auto& addend){
      return std::exchange( addend.Get(), 0.0f );
    }
  );

  // how much do we already have?
  const float current_amount = *begin<dish2::ResourceStockpileWrapper<Spec>>();

  // update stockpiles
  std::fill(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    received_amount + current_amount
  );

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
