#pragma once
#ifndef DISH2_SERVICES_RESOURCESENDINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_RESOURCESENDINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/nan_to_zero.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/isfinite.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../cell/cardinal_iterators/NeighborResourceReceiveResistanceWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceNodeOutputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceReserveRequestWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceSendLimitWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceSendRequestWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Based on writable state within each cardinal, calculates and dispatches
 * resource that should be shared to each neighbor cell.
 */
struct ResourceSendingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_SENDING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "resource sending service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check resource stockpile consistency and validity
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));
    emp_assert( std::all_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      [](const auto amt){ return std::isfinite(amt) && ( amt >= 0 ); }
    ) );
    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isnan( val ); }
    ) );


    // initialize available amount to entire stockpile
    thread_local emp::vector<float> send_amounts;
    send_amounts.clear();
    std::copy(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      std::back_inserter( send_amounts )
    );

    // subtract out how much each cardinal wants to stockpile
    std::transform(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      cell.template begin<dish2::ResourceReserveRequestWrapper<spec_t>>(),
      std::begin( send_amounts ),
      [](const auto send_amount, const auto reserve_request_raw){
        return uitsl::nan_to_zero(
          std::clamp(send_amount - reserve_request_raw.Get(), 0.0f, send_amount)
        );
      }
    );

    // check that each individual send request is leq total available amount
    // within float tolerance
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isnan(amt); }
    ), "a" );
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isinf(amt); }
    ), "a" );
    emp_assert( std::all_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [&cell](const auto amt){
        return amt
          - *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>()
          <= std::numeric_limits<float>::epsilon();
      }
    ), "a" );

    // multiply by fraction requested
    std::transform(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      cell.template begin<dish2::ResourceSendRequestWrapper<spec_t>>(),
      std::begin( send_amounts ),
      [](const auto send_amount, const auto send_request_raw){
        return uitsl::nan_to_zero( std::clamp(
          send_amount * send_request_raw.Get(),
          0.0f, send_amount
        ) );
      }
    );

    // check that each individual send request is leq total available amount
    // within float tolerance
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isnan(amt); }
    ), "b" );
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isinf(amt); }
    ), "b" );
    emp_assert( std::all_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [&cell](const auto amt){
        return amt
          - *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>()
          <= std::numeric_limits<float>::epsilon();
      }
    ), "b" );

    // if absolute rate limited, cap by it
    std::transform(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      cell.template begin<dish2::ResourceSendLimitWrapper<spec_t>>(),
      std::begin( send_amounts ),
      [](const auto send_amount, const auto send_limit_raw){
        const auto send_limit = uitsl::nan_to_zero(
          std::max( 0.0f, send_limit_raw.Get() )
        );
        return send_limit
          ? std::min( send_amount, send_limit )
          : send_amount
        ;
      }
    );

    // check that each individual send request is leq total available amount
    // within float tolerance
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isnan(amt); }
    ), "c" );
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isinf(amt); }
    ), "c" );
    emp_assert( std::all_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [&cell](const auto amt){
        return amt
          - *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>()
          <= std::numeric_limits<float>::epsilon();
      }
    ), "c" );

    // check that all send amounts are non-negative and finite
    emp_assert( std::all_of(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      [](const auto val){ return std::isfinite(val); }
    ) );
    emp_assert( std::all_of(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      [](const auto val){ return val >= 0.0f; }
    ) );

    // multiply each send amount by its fraction of sum send amount
    const auto total_requested = std::accumulate(
      std::begin( send_amounts ), std::end( send_amounts ),
      float{}
    );
    if (total_requested) std::transform(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      std::begin( send_amounts ),
      [total_requested](const auto amt){ return amt * (amt / total_requested); }
    );

    // check that each individual send request is leq total available amount
    // within float tolerance
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isnan(amt); }
    ), "d" );
    emp_assert( std::none_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [](const auto amt){ return std::isinf(amt); }
    ), "d" );
    emp_assert( std::all_of(
      std::begin(send_amounts),
      std::end(send_amounts),
      [&cell](const auto amt){
        return amt
          - *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>()
          <= std::numeric_limits<float>::epsilon();
      }
    ), "d" );

    // check that all send amounts are non-negative and finite
    emp_assert( std::all_of(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      [](const auto val){ return std::isfinite(val); }
    ) );
    emp_assert( std::all_of(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      [](const auto val){ return val >= 0.0f; }
    ) );

    // check that sum send amount doesn't exceed stockpiled amount
    // within float tolerance
    emp_assert( std::nextafter(
      std::accumulate(
        std::begin( send_amounts ), std::end( send_amounts ), 0.0f
      ),
      *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>()
    ) <= *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>() );

    // zero out requests to send to non-alive neighbors
    std::transform(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      cell.template begin<dish2::NeighborIsAliveWrapper<spec_t>>(),
      std::begin( send_amounts ),
      [](const auto send_amount, const auto is_alive){
        return is_alive.Get() ? send_amount : 0.0f;
      }
    );

    // do the send
    float stockpile
      = *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>();
    uitsl::for_each(
      std::begin( send_amounts ),
      std::end( send_amounts ),
      cell.template begin<dish2::ResourceNodeOutputWrapper<spec_t>>(),
      [&stockpile](const float send_amount, auto& resource_output){
        stockpile -= send_amount;
        uitsl_err_audit(!
          resource_output.TryPut( send_amount )
        );
      }
    );

    // patch for precision errors
    // could also clamp, but this avoids branching
    stockpile += std::numeric_limits<float>::epsilon();

    // check that stockpile wasn't overspent or corrupted
    emp_assert( std::isfinite(stockpile) && ( stockpile >= 0.0f ), stockpile );

    // update stockpile state
    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      stockpile
    );

    // check resource stockpile consistency and validity
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));
    emp_assert( std::all_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      [](const auto amt){ return std::isfinite(amt); }
    ) );
    emp_assert( std::all_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      [](const auto amt){ return amt >= 0.0f; }
    ) );
    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isnan( val ); }
    ) );


  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_RESOURCESENDINGSERVICE_HPP_INCLUDE
