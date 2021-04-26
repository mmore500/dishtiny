#pragma once
#ifndef DISH2_UTILITY_STATIC_COARSE_TIMER_HPP_INCLUDE
#define DISH2_UTILITY_STATIC_COARSE_TIMER_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"

namespace dish2 {

const uitsl::CoarseTimer static_coarse_timer;

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_STATIC_COARSE_TIMER_HPP_INCLUDE
