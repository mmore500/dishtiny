#pragma once

#include "sgpl/spec/Spec.hpp"

template< typename OpLibrary >
struct BenchSpec : public sgpl::Spec<OpLibrary> {

  using global_matching_t = emp::MatchDepository<
    unsigned short,
    emp::OptimizedApproxDualStreakMetric<64>,
    emp::statics::RankedSelector<>,
    emp::PlusCountdownRegulator<
      std::deci, // Slope
      std::ratio<1,4>, // MaxUpreg
      std::deci, // ClampLeeway
      2 // CountdownStart
    >,
    true,
    8
  >;

};
