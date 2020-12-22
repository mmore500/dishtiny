#pragma once
#ifndef DISH2_ALGORITHM_TEST_MODULE_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE
#define DISH2_ALGORITHM_TEST_MODULE_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_module.hpp"

#include "../cell/Cell.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "detect_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
bool test_module_for_phenotypic_neutrality(
  const dish2::Genome<Spec>& genome, const size_t module_idx
) {

  const dish2::LogScope guard( "evaluating module ", module_idx );

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  auto nopout = genome;
  nopout.program = sgpl::nop_out_module<sgpl_spec_t>(
    nopout.program, module_idx
  );

  return dish2::detect_phenotypic_divergence<Spec>( genome, nopout );

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_TEST_MODULE_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE
