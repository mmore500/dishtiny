#pragma once
#ifndef DISH2_SERVICES_SPAWNSENDINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_SPAWNSENDINGSERVICE_HPP_INCLUDE

#include <set>

#include "../../../third-party/conduit/include/uitsl/algorithm/copy_if.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cell/cardinal_iterators/GenomeNodeOutputWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../cell/cardinal_iterators/SpawnArrestWrapper.hpp"
#include "../cell/cardinal_iterators/SpawnRequestWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

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

    // check resource stockpile consistency
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<Spec>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
        cell.template end<dish2::ResourceStockpileWrapper<Spec>>()
      ).size() == 1
    ));

    auto available_resource{
      *cell.template begin<dish2::ResourceStockpileWrapper<Spec>>()
    };

    if ( available_resource < 1 ) return;

    thread_local emp::vector< std::reference_wrapper<
      typename dish2::GenomeNodeOutputWrapper<Spec>::value_type
    > > requested_outputs;
    requested_outputs.clear();

    uitsl::copy_if(
      cell.template begin<dish2::GenomeNodeOutputWrapper<Spec>>(),
      cell.template end<dish2::GenomeNodeOutputWrapper<Spec>>(),
      cell.template begin<dish2::SpawnArrestWrapper<Spec>>(),
      cell.template begin<dish2::SpawnRequestWrapper<Spec>>(),
      std::back_inserter( requested_outputs ),
      []( const auto arrest, const auto request ){ return request && !arrest; }
    );

    while ( available_resource >= 1 && requested_outputs.size() ) {

      // pick a random request
      const size_t idx = sgpl::ThreadLocalRandom::Get().GetUInt(
        requested_outputs.size()
      );

      // do the spawn send
      requested_outputs[ idx ].get().Put( *cell.genome );
      available_resource -= 1;

      // swap 'n pop request
      requested_outputs[ idx ] = requested_outputs.back();
      requested_outputs.pop_back();

    }

    std::fill(
      cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
      cell.template end<dish2::ResourceStockpileWrapper<Spec>>(),
      available_resource
    );

    // check resource stockpile consistency
    emp_assert((
      std::set< typename dish2::ResourceStockpileWrapper<Spec>::value_type >(
        cell.template begin<dish2::ResourceStockpileWrapper<Spec>>(),
        cell.template end<dish2::ResourceStockpileWrapper<Spec>>()
      ).size() == 1
    ));

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_SPAWNSENDINGSERVICE_HPP_INCLUDE
