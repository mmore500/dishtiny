#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_HEIRPAYOUTROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_HEIRPAYOUTROUTINE_HPP_INCLUDE

#include <cmath>
#include <utility>

#include "../../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../../config/cfg.hpp"
#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../cardinal_iterators/ApoptosisRequestWrapper.hpp"
#include "../cardinal_iterators/HeirRequestWrapper.hpp"
#include "../cardinal_iterators/ResourceNodeOutputWrapper.hpp"
#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../../debug/LogScope.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::HeirPayoutRoutine() {

  const dish2::LogScope guard{ "heir payout routine", "TODO", 3 };

  // calculate amount of resource to send each heir
  const size_t num_heirs = std::count_if(
    begin<dish2::HeirRequestWrapper<Spec>>(),
    end<dish2::HeirRequestWrapper<Spec>>(),
    std::identity
  );

  if ( num_heirs == 0 ) return;

  // stockpiled resource plus resource recovered via apoptosis, if applicable
  const float amt_total =
    *begin<dish2::ResourceStockpileWrapper<Spec>>()
    + dish2::cfg.APOP_RECOVERY_FRAC() * std::any_of(
      begin<dish2::ApoptosisRequestWrapper<Spec>>(),
      end<dish2::ApoptosisRequestWrapper<Spec>>(),
      std::identity
    );

  emp_assert( std::isfinite( amt_total ), amt_total );

  const float amt_per_heir = amt_total / num_heirs;

  emp_assert( std::isfinite( amt_per_heir ), amt_per_heir );

  // send resource to heirs
  uitsl::for_each(
    begin<dish2::HeirRequestWrapper<Spec>>(),
    end<dish2::HeirRequestWrapper<Spec>>(),
    begin<dish2::ResourceNodeOutputWrapper<Spec>>(),
    [amt_per_heir](const auto is_heir, auto& output) {
      if (is_heir) output.TryPut( amt_per_heir );
    }
  );

  // clear sent resource
  std::fill(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    0.0f
  );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_HEIRPAYOUTROUTINE_HPP_INCLUDE
