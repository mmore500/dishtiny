#pragma once
#ifndef DISH2_UTILITY_CALC_FITNESS_DIFFERENTIAL_HPP_INCLUDE
#define DISH2_UTILITY_CALC_FITNESS_DIFFERENTIAL_HPP_INCLUDE

#include <limits>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

double calc_fitness_differential(
  const size_t update, const double prevalence, const size_t num_roots
) {

  const double initial_prevalence = 1.0 / num_roots;

  emp_assert( update );
  const double growth_rate = (prevalence - initial_prevalence) / update;

  return growth_rate;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_CALC_FITNESS_DIFFERENTIAL_HPP_INCLUDE
