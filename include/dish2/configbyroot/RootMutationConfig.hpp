#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOTMUTATIONCONFIG_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOTMUTATIONCONFIG_HPP_INCLUDE

#include <cstddef>
#include <map>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

struct RootMutationConfig {

  double point_mutation_rate_multiplicand{ 1.0 };
  double sequence_defect_rate_multiplicand{ 1.0 };
  double mutation_occurence_rate_multiplicand{ 1.0 };

  emp::optional<size_t> program_max_size_override{};

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

  std::map<std::string, std::string> MakeSummary() const {
    return {
      {"Point Mutation Rate Multiplicand",
        emp::to_string(point_mutation_rate_multiplicand)},
      {"Sequence Defect Rate Multiplicand",
        emp::to_string(sequence_defect_rate_multiplicand)},
      {"Mutation Occurence Rate Multiplicand",
        emp::to_string(mutation_occurence_rate_multiplicand)},
      {"Program Max Size Override",
        emp::to_string(program_max_size_override)},
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOTMUTATIONCONFIG_HPP_INCLUDE
