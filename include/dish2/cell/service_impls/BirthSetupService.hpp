#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_BIRTHSETUPSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_BIRTHSETUPSERVICE_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/Empirical/source/base/vector.h"
#include "../../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../cardinal_iterators/GenomeNodeInputWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::BirthSetupService() {

  // TODO check quiescence period?

  thread_local emp::vector< std::reference_wrapper<
    typename dish2::GenomeNodeInputWrapper<Spec>::value_type
  > > fresh_inputs;
  fresh_inputs.clear();

  std::copy_if(
    begin<dish2::GenomeNodeInputWrapper<Spec>>(),
    end<dish2::GenomeNodeInputWrapper<Spec>>(),
    std::back_inserter( fresh_inputs ),
    [](auto& input){ return input.Jump(); }
  );

  if ( fresh_inputs.size() ) {
    DeathRoutine();
    genome = fresh_inputs[
      sgpl::ThreadLocalRandom::Get().GetUInt( fresh_inputs.size() )
    ].get().Get();
    MakeAliveRoutine();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_BIRTHSETUPSERVICE_HPP_INCLUDE
