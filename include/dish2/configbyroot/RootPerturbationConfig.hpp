#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

struct RootPerturbationConfig {

  double intermittent_introspective_state_exchange_probability{};
  double intermittent_introspective_state_rotate_probability{};

  emp::optional<size_t> introspective_state_target_idx;

  double intermittent_writable_state_exchange_probability{};
  double intermittent_writable_state_rotate_probability{};

  emp::optional<size_t> writable_state_target_idx;

  bool ShouldExchangeIntrospectiveState() const { return sgpl::tlrand.Get().P(
    intermittent_introspective_state_exchange_probability
  ); }

  bool ShouldRotateIntrospectiveState() const { return sgpl::tlrand.Get().P(
    intermittent_introspective_state_rotate_probability
  ); }

  bool ShouldExchangeWritableState() const { return sgpl::tlrand.Get().P(
    intermittent_writable_state_exchange_probability
  ); }

  bool ShouldRotateWritableState() const {
    return sgpl::tlrand.Get().P(intermittent_writable_state_rotate_probability);
  }

  std::map<std::string, std::string> MakeSummary() const {
    return {
      {"Intermittent Introspective State Exchange Probability",
        emp::to_string(intermittent_introspective_state_exchange_probability)},
      {"Intermittent Introspective State Rotate Probability",
        emp::to_string(intermittent_introspective_state_rotate_probability)},
      {"Introspective State Target Idx",
        emp::to_string(introspective_state_target_idx)},
      {"Intermittent Writable State Exchange Probability",
        emp::to_string(intermittent_writable_state_exchange_probability)},
      {"Intermittent Writable State Rotate Probability",
        emp::to_string(intermittent_writable_state_rotate_probability)},
      {"Writable State Target Idx",
        emp::to_string(writable_state_target_idx)},
    };
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE
