#pragma once
#ifndef DISH2_LOAD_DOSE_INNOCULUM_BUCKETS_HPP_INCLUDE
#define DISH2_LOAD_DOSE_INNOCULUM_BUCKETS_HPP_INCLUDE

#include <algorithm>
#include <cstddef>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../debug/log_msg.hpp"
#include "../genome/Genome.hpp"

namespace dish2 {

template<typename Spec>
emp::vector<emp::vector<dish2::Genome<Spec>>>
dose_innoculum_buckets(
  emp::vector<emp::vector<dish2::Genome<Spec>>> innoculum_buckets,
  emp::vector<size_t> innoculum_doses
) {
  emp_assert(innoculum_buckets.size() == innoculum_doses.size());
  if (innoculum_doses.empty()) return {};

  for (auto& bucket : innoculum_buckets) {
    emp::Shuffle( sgpl::tlrand.Get(), bucket );
  }

  const size_t max_dose = *std::max_element(
    std::begin(innoculum_doses),
    std::end(innoculum_doses)
  );
  dish2::log_msg( "max bucket dose is ", max_dose );


  emp::vector<emp::vector<dish2::Genome<Spec>>> dosed_innoculum_buckets;
  for (size_t dose{}; dose < max_dose; ++dose) {
    for (
      size_t which_bucket{};
      which_bucket < innoculum_buckets.size();
      ++which_bucket
    ) {
      emp_assert(innoculum_buckets.size() == innoculum_doses.size());

      const size_t bucket_dose = innoculum_doses[which_bucket];
      if (dose >= bucket_dose) continue;

      const auto& bucket = innoculum_buckets[which_bucket];

      emp_assert(dose < bucket_dose);
      dosed_innoculum_buckets.emplace_back();
      for (
        size_t sample_idx = dose % bucket.size();
        sample_idx < bucket.size();
        sample_idx += bucket_dose
      ) dosed_innoculum_buckets.back().push_back(bucket[sample_idx]);

    }
  }


  return dosed_innoculum_buckets;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_DOSE_INNOCULUM_BUCKETS_HPP_INCLUDE
