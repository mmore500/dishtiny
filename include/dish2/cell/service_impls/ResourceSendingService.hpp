#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCESENDINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_RESOURCESENDINGSERVICE_HPP_INCLUDE

#include <cmath>
#include <functional>
#include <limits>
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

  // check resource stockpile consistency and validity
  emp_assert((
    std::set< typename dish2::ResourceStockpileWrapper<Spec>::value_type >(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
    ).size() == 1
  ));
  emp_assert( std::all_of(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    [this](const auto amt){ return std::isfinite(amt) && ( amt >= 0 ); }
  ) );

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
      return std::clamp(send_amount - reserve_request.Get(), 0.0f, send_amount);
    }
  );

  // check that each individual send request is leq total available amount
  // within float tolerance
  emp_assert( std::all_of(
    std::begin(send_amounts),
    std::end(send_amounts),
    [this](const auto amt){
      return std::nextafter(
        amt, *begin<dish2::ResourceStockpileWrapper<Spec>>()
      ) <= *begin<dish2::ResourceStockpileWrapper<Spec>>();
    }
  ), "a" );

  // multiply by fraction requested
  std::transform(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    begin<dish2::ResourceSendRequestWrapper<Spec>>(),
    std::begin( send_amounts ),
    [](const auto send_amount, const auto send_request_raw){
      // precision workaround, get next smallest value below 1.0f
      constexpr float top = 1.0f - std::numeric_limits<float>::epsilon();
      const auto send_request = std::clamp( send_request_raw.Get(), 0.0f, top );
      return send_amount * send_request;
    }
  );

  // check that each individual send request is leq total available amount
  // within float tolerance
  emp_assert( std::all_of(
    std::begin(send_amounts),
    std::end(send_amounts),
    [this](const auto amt){
      return std::nextafter(
        amt, *begin<dish2::ResourceStockpileWrapper<Spec>>()
      ) <= *begin<dish2::ResourceStockpileWrapper<Spec>>();
    }
  ), "b" );

  // if absolute rate limited, cap by it
  std::transform(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    begin<dish2::ResourceSendLimitWrapper<Spec>>(),
    std::begin( send_amounts ),
    [](const auto send_amount, const auto send_limit_raw){
      const auto send_limit = std::max( 0.0f, send_limit_raw.Get() );
      return send_limit
        ? std::min( send_amount, send_limit )
        : send_amount
      ;
    }
  );

  // check that each individual send request is leq total available amount
  // within float tolerance
  emp_assert( std::all_of(
    std::begin(send_amounts),
    std::end(send_amounts),
    [this](const auto amt){
      return std::nextafter(
        amt, *begin<dish2::ResourceStockpileWrapper<Spec>>()
      ) <= *begin<dish2::ResourceStockpileWrapper<Spec>>();
    }
  ), "c" );

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
    [total_requested](const auto amt){ return amt * (amt / total_requested); }
  );

  // check that each individual send request is leq total available amount
  // within float tolerance
  emp_assert( std::all_of(
    std::begin(send_amounts),
    std::end(send_amounts),
    [this](const auto amt){
      return std::nextafter(
        amt, *begin<dish2::ResourceStockpileWrapper<Spec>>()
      ) <= *begin<dish2::ResourceStockpileWrapper<Spec>>();
    }
  ), "d" );

  // check that all send amounts are non-negative and finite
  emp_assert( std::all_of(
    std::begin( send_amounts ),
    std::end( send_amounts ),
    [](const auto val){ return std::isfinite(val) && ( val >= 0 ); }
  ) );

  // check that sum send amount doesn't exceed stockpiled amount
  // within float tolerance
  emp_assert( std::nextafter(
    std::accumulate(
      std::begin( send_amounts ), std::end( send_amounts ), 0.0f
    ),
    *begin<dish2::ResourceStockpileWrapper<Spec>>()
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

  // patch for precision errors
  // could also clamp, but this avoids branching
  stockpile += std::numeric_limits<float>::epsilon();

  // check that stockpile wasn't overspent or corrupted
  emp_assert( std::isfinite(stockpile) && ( stockpile >= 0.0f ), stockpile );

  // update stockpile state
  std::fill(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    stockpile
  );

  // check resource stockpile consistency and validity
  emp_assert((
    std::set< typename dish2::ResourceStockpileWrapper<Spec>::value_type >(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
    ).size() == 1
  ));
  emp_assert( std::all_of(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    [this](const auto amt){ return std::isfinite(amt) && ( amt >= 0 ); }
  ) );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_RESOURCESENDINGSERVICE_HPP_INCLUDE
