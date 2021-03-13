#pragma once
#ifndef DISH2_GENOME_CONFIGCUSTOMIZATIONS_HPP_INCLUDE
#define DISH2_GENOME_CONFIGCUSTOMIZATIONS_HPP_INCLUDE

#include <cstddef>
#include <tuple>

#include "../../../third-party/cereal/include/cereal/cereal.hpp"
#include "../../../third-party/cereal/include/cereal/types/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

struct ConfigCustomizations {

  double point_mutation_rate_multiplicand{ 1.0 };
  double sequence_defect_rate_multiplicand{ 1.0 };
  double mutation_occurence_rate_multiplicand{ 1.0 };

  emp::optional<size_t> program_max_size_override{};

  double intermittent_cpu_reset_probability{};
  double intermittent_writable_state_rotate_probability{};
  double intermittent_writable_state_exchange_probability{};

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

  size_t GetProgramMaxSize() const {
    return program_max_size_override.value_or(
      dish2::cfg.PROGRAM_MAX_SIZE()
    );
  }

  bool ShouldResetCpu() const {
    return sgpl::tlrand.Get().P( intermittent_cpu_reset_probability );
  }

  bool ShouldRotateWritableState() const {
    return sgpl::tlrand.Get().P(intermittent_writable_state_rotate_probability);
  }

  bool ShouldExchangeWritableState() const { return sgpl::tlrand.Get().P(
    intermittent_writable_state_exchange_probability
  ); }

  bool operator==(const ConfigCustomizations& other) const {
    return std::tuple{
      point_mutation_rate_multiplicand,
      sequence_defect_rate_multiplicand,
      mutation_occurence_rate_multiplicand,
      program_max_size_override,
      intermittent_cpu_reset_probability,
      intermittent_writable_state_rotate_probability,
      intermittent_writable_state_exchange_probability
    } == std::tuple{
      other.point_mutation_rate_multiplicand,
      other.sequence_defect_rate_multiplicand,
      other.mutation_occurence_rate_multiplicand,
      other.program_max_size_override,
      other.intermittent_cpu_reset_probability,
      other.intermittent_writable_state_rotate_probability,
      other.intermittent_writable_state_exchange_probability
    };
  }

  bool operator<(const ConfigCustomizations& other) const {
    return std::tuple{
      point_mutation_rate_multiplicand,
      sequence_defect_rate_multiplicand,
      mutation_occurence_rate_multiplicand,
      program_max_size_override,
      intermittent_cpu_reset_probability,
      intermittent_writable_state_rotate_probability,
      intermittent_writable_state_exchange_probability,
    } < std::tuple{
      other.point_mutation_rate_multiplicand,
      other.sequence_defect_rate_multiplicand,
      other.mutation_occurence_rate_multiplicand,
      other.program_max_size_override,
      other.intermittent_cpu_reset_probability,
      other.intermittent_writable_state_rotate_probability,
      other.intermittent_writable_state_exchange_probability
    };
  }

  template<typename Archive> void serialize( Archive & ar ) { ar(
    CEREAL_NVP( point_mutation_rate_multiplicand ),
    CEREAL_NVP( sequence_defect_rate_multiplicand ),
    CEREAL_NVP( mutation_occurence_rate_multiplicand ),
    CEREAL_NVP( program_max_size_override ),
    CEREAL_NVP( intermittent_cpu_reset_probability ),
    CEREAL_NVP( intermittent_writable_state_rotate_probability ),
    CEREAL_NVP( intermittent_writable_state_exchange_probability )

  ); }

};

} // namespace dish2

#endif // #ifndef DISH2_GENOME_CONFIGCUSTOMIZATIONS_HPP_INCLUDE
