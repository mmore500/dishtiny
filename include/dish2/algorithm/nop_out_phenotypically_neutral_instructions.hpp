#pragma once
#ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
#define DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/vector.h"

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/Cell.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "detect_phenotypic_divergence.hpp"
#include "nop_out_phenotypically_neutral_modules.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> nop_out_phenotypically_neutral_instructions(
  const dish2::Genome<Spec>& genome
) {

  const dish2::LogScope guard1{ "noping out neutral sites" };

  auto res = dish2::nop_out_phenotypically_neutral_modules<Spec>( genome );

  const dish2::LogScope guard2{ "evaluating instruction-by-instruction" };

  emp::vector< size_t > neutral_sites;

  for (size_t site{}; site < res.program.size(); ++site) {

    const dish2::LogScope guard3{ emp::to_string("evaluating site ", site) };

    auto test = res;

    test.program[site].NopOut();

    if ( test == res ) {
      dish2::log_event({dish2::result_skip, "inst alread nop, skipping test"});
      continue;
    }

    if ( !dish2::detect_phenotypic_divergence<Spec>( test, res ) ) {
      dish2::log_event({ dish2::info, emp::to_string("nopping site ", site) });
      neutral_sites.push_back( site );
    }

  }

  for ( const auto site : neutral_sites ) res.program[site].NopOut();

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
