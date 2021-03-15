#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

struct RootPerturbationConfig {

  double intermittent_writable_state_exchange_probability{};
  double intermittent_writable_state_rotate_probability{};

  emp::optional<size_t> writable_state_target_idx;

  bool ShouldExchangeWritableState() const { return sgpl::tlrand.Get().P(
    intermittent_writable_state_exchange_probability
  ); }

  bool ShouldRotateWritableState() const {
    return sgpl::tlrand.Get().P(intermittent_writable_state_rotate_probability);
  }

  std::map<std::string, std::string> MakeSummary() const {
    return {
      {"Intermittent Writable State Exchange Probability",
        emp::to_string(intermittent_writable_state_exchange_probability)},
      {"Intermittent Writable State Rotate Probability",
        emp::to_string(intermittent_writable_state_rotate_probability)},
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE
