#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_SPAWNSENDINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_SPAWNSENDINGSERVICE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/algorithm/copy_if.hpp"
#include "../../../../third-party/Empirical/source/base/vector.h"
#include "../../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cardinal_iterators/GenomeNodeOutputWrapper.hpp"
#include "../cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../cardinal_iterators/SpawnArrestWrapper.hpp"
#include "../cardinal_iterators/SpawnRequestWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::SpawnSendingService() {

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));

  auto available_resource{ *begin<dish2::ResourceStockpileWrapper<Spec>>() };

  if ( available_resource < 1 ) return;

  thread_local emp::vector< std::reference_wrapper<
    typename dish2::GenomeNodeOutputWrapper<Spec>::value_type
  > > requested_outputs;
  requested_outputs.clear();

  uitsl::copy_if(
    begin<dish2::GenomeNodeOutputWrapper<Spec>>(),
    end<dish2::GenomeNodeOutputWrapper<Spec>>(),
    begin<dish2::SpawnArrestWrapper<Spec>>(),
    begin<dish2::SpawnRequestWrapper<Spec>>(),
    std::back_inserter( requested_outputs ),
    []( const auto arrest, const auto request ){ return request && !arrest; }
  );

  while ( available_resource >= 1 && requested_outputs.size() ) {

    // pick a random request
    const size_t idx = sgpl::ThreadLocalRandom::Get().GetUInt(
      requested_outputs.size()
    );

    // do the spawn send
    requested_outputs[ idx ].get().Put( *genome );
    available_resource -= 1;

    // swap 'n pop request
    requested_outputs[ idx ] = requested_outputs.back();
    requested_outputs.pop_back();

  }

  std::fill(
    begin<dish2::ResourceStockpileWrapper<Spec>>(),
    end<dish2::ResourceStockpileWrapper<Spec>>(),
    available_resource
  );

  // check resource stockpile consistency
  emp_assert(( std::set(
      begin<dish2::ResourceStockpileWrapper<Spec>>(),
      end<dish2::ResourceStockpileWrapper<Spec>>()
  ).size() == 1 ));


}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_SPAWNSENDINGSERVICE_HPP_INCLUDE
