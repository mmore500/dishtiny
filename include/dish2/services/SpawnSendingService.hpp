#pragma once
#ifndef DISH2_SERVICES_SPAWNSENDINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_SPAWNSENDINGSERVICE_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <numeric>
#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/copy_if.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/GenomeNodeOutputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../cell/cardinal_iterators/SpawnArrestWrapper.hpp"
#include "../cell/cardinal_iterators/SpawnCountWrapper.hpp"
#include "../cell/cardinal_iterators/SpawnRequestWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../runninglog/SpawnEvent.hpp"

namespace dish2 {

/**
 * Dispatches cellular reproduction.
 *
 * If available resource is greater than or equal to 1.0, iterates randomly
 * through every cardinal to determine whether it requested to spawn and has
 * not arrested spawning. Then, one of these requests is dispatched at random
 * and stockpile is decreased by 1.0.
 */
struct SpawnSendingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.SPAWN_SENDING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "spawn sending service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check resource stockpile consistency
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));
    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isnan( val ); }
    ) );

    // check spawn count consistency
    emp_assert((
      std::set< typename dish2::SpawnCountWrapper<spec_t>::value_type >(
        cell.template begin<dish2::SpawnCountWrapper<spec_t>>(),
        cell.template end<dish2::SpawnCountWrapper<spec_t>>()
      ).size() == 1
    ));

    float available_resource{
      *cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>()
    };

    emp_assert( std::isfinite( available_resource ), available_resource );

    if ( available_resource < 1 ) return;

    thread_local emp::vector< std::reference_wrapper<
      typename dish2::GenomeNodeOutputWrapper<spec_t>::value_type
    > > requested_outputs;
    requested_outputs.clear();

    uitsl::copy_if(
      cell.template begin<dish2::GenomeNodeOutputWrapper<spec_t>>(),
      cell.template end<dish2::GenomeNodeOutputWrapper<spec_t>>(),
      cell.template begin<dish2::SpawnArrestWrapper<spec_t>>(),
      cell.template begin<dish2::SpawnRequestWrapper<spec_t>>(),
      std::back_inserter( requested_outputs ),
      []( const auto arrest, const auto request ){ return request && !arrest; }
    );

    while ( available_resource >= 1 && requested_outputs.size() ) {

      // pick a random request
      const size_t idx = sgpl::tlrand.Get().GetUInt(
        requested_outputs.size()
      );

      // do the spawn send
      requested_outputs[ idx ].get().Put( *cell.genome );
      available_resource -= 1;

      // record spawn send in spawn count
      std::transform(
        cell.template begin<dish2::SpawnCountWrapper<spec_t>>(),
        cell.template end<dish2::SpawnCountWrapper<spec_t>>(),
        cell.template begin<dish2::SpawnCountWrapper<spec_t>>(),
        []( const auto& cur ){ emp_assert( cur + 1 > cur ); return cur + 1; }
      );

      // record spawn event in running log
      const auto& peripheral = cell.cardinals[idx].peripheral;
      const size_t replev = peripheral.readable_state.template Get<
        dish2::RepLevRequest<spec_t>
      >().GetRepLev();
      const size_t kin_id_commonality_parent_daughter = spec_t::NLEV - replev;
      const size_t kin_id_commonality_parent_eliminated =
        peripheral.readable_state.template Get<
          dish2::KinGroupIDView<spec_t>
        >().CountCommonality(
          std::as_const( peripheral.state_node_input ).Get().template Get<
            dish2::KinGroupIDView<spec_t>
          >()
        );
      const size_t kin_id_commonality_daughter_eliminated = std::min(
        kin_id_commonality_parent_daughter,
        kin_id_commonality_parent_eliminated
      );

      cell.running_logs.Record( dish2::SpawnEvent<spec_t>{
        kin_id_commonality_daughter_eliminated,
        kin_id_commonality_parent_daughter,
        kin_id_commonality_parent_eliminated,
        cell.cardinals.size(),
        cell.GetPeripherality(),
        replev
      } );

      // swap 'n pop request
      requested_outputs[ idx ] = requested_outputs.back();
      requested_outputs.pop_back();

    }

    emp_assert(std::isfinite( available_resource ), available_resource);

    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      available_resource
    );

    // check resource stockpile consistency
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<spec_t>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
        cell.template end<dish2::ResourceStockpileWrapper<spec_t>>()
      ).size() == 1
    ));
    emp_assert( std::none_of(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      []( const auto val ){ return std::isinf( val ); }
    ) );

    // check spawn count consistency
    emp_assert((
      std::set< typename dish2::SpawnCountWrapper<spec_t>::value_type >(
        cell.template begin<dish2::SpawnCountWrapper<spec_t>>(),
        cell.template end<dish2::SpawnCountWrapper<spec_t>>()
      ).size() == 1
    ));

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_SPAWNSENDINGSERVICE_HPP_INCLUDE
