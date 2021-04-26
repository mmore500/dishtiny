#pragma once
#ifndef DISH2_ALGORITHM_DETECT_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
#define DISH2_ALGORITHM_DETECT_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE

#include "run_until_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
bool detect_phenotypic_divergence(
  dish2::ThreadWorld<Spec>& world1,
  dish2::ThreadWorld<Spec>& world2
) {

  const auto res = dish2::run_until_phenotypic_divergence<Spec>(world1, world2);

  return res != dish2::cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES();

}

template< typename Spec >
bool detect_phenotypic_divergence(
  const dish2::Genome<Spec>& genome1,
  const dish2::Genome<Spec>& genome2
) {

  const auto res = dish2::run_until_phenotypic_divergence<Spec>(
    genome1, genome2
  );

  return res != dish2::cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES();

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_DETECT_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
