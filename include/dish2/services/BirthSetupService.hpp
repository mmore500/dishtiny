#pragma once
#ifndef DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE

#include <functional>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"
#include "../cell/cardinal_iterators/GenomeNodeInputWrapper.hpp"

namespace dish2 {

struct BirthSetupService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.BIRTH_SETUP_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {
    // TODO check quiescence period?

    thread_local emp::vector< std::reference_wrapper<
      typename dish2::GenomeNodeInputWrapper<Spec>::value_type
    > > fresh_inputs;
    fresh_inputs.clear();

    std::copy_if(
      cell.template begin<dish2::GenomeNodeInputWrapper<Spec>>(),
      cell.template end<dish2::GenomeNodeInputWrapper<Spec>>(),
      std::back_inserter( fresh_inputs ),
      [](auto& input){ return input.Jump(); }
    );

    if ( fresh_inputs.size() ) {
      cell.DeathRoutine();
      cell.genome = fresh_inputs[
        sgpl::ThreadLocalRandom::Get().GetUInt( fresh_inputs.size() )
      ].get().Get();
      cell.genome->ElapseGeneration( 0 ); // TODO FIXME
      cell.MakeAliveRoutine();
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_BIRTHSETUPSERVICE_HPP_INCLUDE
