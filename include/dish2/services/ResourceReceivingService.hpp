#pragma once
#ifndef DISH2_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/ReceivedResourceFromWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceInputPeekWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceNodeInputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceNodeOutputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Calculates total amount of resource received across every cardinal, and then
 * adds that total to resource stockpile.
 *
 * If the cell is not alive, it instead refunds all received resources back to
 * each sending cell.
 */
struct ResourceReceivingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_RECEIVING_SERVICE_FREQUENCY();
    // must run whether cell is alive or not to keep aggregated ducts in sync
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "resource receiving service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // write amount of incoming resource readable state
    std::copy(
      cell.template begin<dish2::ResourceInputPeekWrapper<spec_t>>(),
      cell.template end<dish2::ResourceInputPeekWrapper<spec_t>>(),
      cell.template begin<dish2::ReceivedResourceFromWrapper<spec_t>>()
    );

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    // dead cells should refund received resources and then return
    if ( !cell.IsAlive() ) {
      uitsl::for_each(
        cell.template begin<dish2::ResourceNodeInputWrapper<spec_t>>(),
        cell.template end<dish2::ResourceNodeInputWrapper<spec_t>>(),
        cell.template begin<dish2::ResourceNodeOutputWrapper<spec_t>>(),
        [](auto& input, auto& output){ uitsl_err_audit(!
          output.TryPut( input.JumpGet() )
        ); }
      );
      return;
    }


    // how much resource have we received across all cardinals?
    const float received_amount = std::accumulate(
      cell.template begin<dish2::ResourceNodeInputWrapper<spec_t>>(),
      cell.template end<dish2::ResourceNodeInputWrapper<spec_t>>(),
      float{},
      [](const float sum, auto& input){ return sum + input.JumpGet(); }
    ) * dish2::cfg.RESOURCE_DECAY() * dish2::cfg.RESOURCE_DECAY();

    emp_assert( std::isfinite( received_amount ), received_amount );
    emp_assert( received_amount >= 0.0f, received_amount );

    // how much do we already have?
    const float current_amount
      = *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>();

    emp_assert( std::isfinite( current_amount ), current_amount );

    const float updated_amount = received_amount + current_amount;

    // update stockpiles
    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      updated_amount
    );

    // check resource stockpile consistency
    emp_assert((
      std::set<typename dish2::ResourceStockpileWrapper<spec_t>::value_type>(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));

    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isnan( val ); }
    ), received_amount, current_amount );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_RESOURCERECEIVINGSERVICE_HPP_INCLUDE
