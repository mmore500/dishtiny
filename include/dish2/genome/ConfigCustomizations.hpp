#pragma once
#ifndef DISH2_GENOME_CONFIGCUSTOMIZATIONS_HPP_INCLUDE
#define DISH2_GENOME_CONFIGCUSTOMIZATIONS_HPP_INCLUDE

#include <cstddef>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

struct ConfigCustomizations {

  double point_mutation_rate_multiplicand{ 1.0 };
  double sequence_defect_rate_multiplicand{ 1.0 };
  double mutation_occurence_rate_multiplicand{ 1.0 };

  double CalcPointMutationRate() const {
    const double res = dish2::cfg.POINT_MUTATION_RATE()
      * point_mutation_rate_multiplicand;
    emp_assert( std::clamp( res, 0.0, 1.0 ) == res );
    return res;
  }

  double CalcSequenceDefectRate() const {
    const double res = dish2::cfg.SEQUENCE_DEFECT_RATE()
      * sequence_defect_rate_multiplicand;
    emp_assert( std::clamp( res, 0.0, 1.0 ) == res );
    return res;
  }

  double CalcMutationOccurenceRate(const size_t rep_lev) const {
    const double res = dish2::cfg.MUTATION_RATE()[rep_lev]
      * mutation_occurence_rate_multiplicand;
    emp_assert( std::clamp( res, 0.0, 1.0 ) == res );
    return res;
  }

  bool operator==(const ConfigCustomizations& other) const {
    return std::tuple{
      point_mutation_rate_multiplicand,
      sequence_defect_rate_multiplicand,
      mutation_occurence_rate_multiplicand
    } == std::tuple{
      other.point_mutation_rate_multiplicand,
      other.sequence_defect_rate_multiplicand,
      other.mutation_occurence_rate_multiplicand
    };
  }

  bool operator<(const ConfigCustomizations& other) const {
    return std::tuple{
      point_mutation_rate_multiplicand,
      sequence_defect_rate_multiplicand,
      mutation_occurence_rate_multiplicand
    } < std::tuple{
      other.point_mutation_rate_multiplicand,
      other.sequence_defect_rate_multiplicand,
      other.mutation_occurence_rate_multiplicand
    };
  }

  template<typename Archive> void serialize( Archive & ar ) { ar(
    CEREAL_NVP( point_mutation_rate_multiplicand ),
    CEREAL_NVP( sequence_defect_rate_multiplicand ),
    CEREAL_NVP( mutation_occurence_rate_multiplicand )
  ); }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_CONFIGCUSTOMIZATIONS_HPP_INCLUDE
