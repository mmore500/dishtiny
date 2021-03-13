#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

namespace dish2 {

struct RootPerturbationConfig {

  double intermittent_cpu_reset_probability{};
  double intermittent_writable_state_rotate_probability{};
  double intermittent_writable_state_exchange_probability{};

  bool ShouldResetCpu() const {
    return sgpl::tlrand.Get().P( intermittent_cpu_reset_probability );
  }

  bool ShouldRotateWritableState() const {
    return sgpl::tlrand.Get().P(intermittent_writable_state_rotate_probability);
  }

  bool ShouldExchangeWritableState() const { return sgpl::tlrand.Get().P(
    intermittent_writable_state_exchange_probability
  ); }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOTPERTURBATIONCONFIG_HPP_INCLUDE
