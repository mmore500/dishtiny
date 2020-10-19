#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCESENDINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_RESOURCESENDINGSERVICE_HPP_INCLUDE

#include <cmath>
#include <functional>
#include <set>
#include <utility>

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"

#include "../cardinal_iterators/NeighborResourceReceiveResistanceWrapper.hpp"
#include "../cardinal_iterators/ResourceNodeOutputWrapper.hpp"
#include "../cardinal_iterators/ResourceReserveRequestWrapper.hpp"
#include "../cardinal_iterators/ResourceSendLimitWrapper.hpp"
#include "../cardinal_iterators/ResourceSendRequestWrapper.hpp"
#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::ResourceSendingService() {

  // check resource stockpile consistency
  emp_assert((
    std::set<decltype(*begin<dish2::ResourceStockpileWrapper<Spec>>())>(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
    ).size() == 1
  ));

  // initialize available amount to entire stockpile
  thread_local emp::vector<float> send_amounts;
  send_amounts.clear();
  std::copy(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    std::back_inserter( send_amounts )
  );

  // subtract out how much each cardinal wants to stockpile
  std::transform(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    begin<dish2::ResourceReserveRequestWrapper<Spec>>(),
    std::begin( send_amounts ),
    [](const auto send_amount, const auto reserve_request){
      return send_amount - std::min( reserve_request.Get(), send_amount );
    }
  );

  // multiply by fraction requested
  std::transform(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    begin<dish2::ResourceSendRequestWrapper<Spec>>(),
    std::begin( send_amounts ),
    [](const auto send_amount, const auto send_request_raw){
      const auto send_request = std::min( 1.0f, std::abs( send_request_raw ) );
      emp_assert( std::clamp( send_request, 0.0f, 1.0f ) == send_request );
      return send_amount * send_request;
    }
  );

  // if absolute rate limited, cap by it
  std::transform(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    begin<dish2::ResourceSendLimitWrapper<Spec>>(),
    std::begin( send_amounts ),
    [](const auto send_amount, const auto send_limit_raw){
      const auto send_limit = std::abs( send_limit_raw );
      return send_limit
        ? std::min( send_amount, send_limit )
        : send_amount
      ;
    }
  );

  // check that all send amounts are non-negative and finite
  emp_assert( std::all_of(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    [](const auto val){ return std::isfinite(val) && ( val >= 0 ); }
  ) );

  // multiply each send amount by its fraction of sum send amount
  const auto total_requested = std::accumulate(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    0.0f
  );
  if (total_requested) std::transform(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    std::begin( send_amounts ),
    [total_requested](const auto amt){ return amt / total_requested; }
  );

  // check that all send amounts are non-negative and finite
  emp_assert( std::all_of(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    [](const auto val){ return std::isfinite(val) && ( val >= 0 ); }
  ) );

  // check that sum send amount doesn't exceed stockpiled amount
  emp_assert( std::accumulate(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    0.0f
  ) <= *begin<dish2::ResourceStockpileWrapper<Spec>>() );

  // do the send
  // TODO write a custom transform-like for_each
  float stockpile = *begin<dish2::ResourceStockpileWrapper<Spec>>();
  for (size_t i{}; i < send_amounts.size(); ++i) {
    auto& resource_output
      = *( begin<dish2::ResourceNodeOutputWrapper<Spec>>() + i );
    const auto send_amount = *( std::begin( send_amounts ) + i );

    stockpile -= send_amount;
    uitsl::err_audit(!
      resource_output.TryPut( send_amount )
    );
  }

  // check that stockpile wasn't overspent or corrupted
  emp_assert( std::isfinite(stockpile) && ( stockpile >= 0 ) );

  // update stockpile state
  std::fill(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    stockpile
  );

  // check resource stockpile consistency
  emp_assert((
    std::set<decltype(*begin<dish2::ResourceStockpileWrapper<Spec>>())>(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
    ).size() == 1
  ));

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCESENDINGSERVICE_HPP_INCLUDE
