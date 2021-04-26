#pragma once
#ifndef DISH2_DEBUG_LOG_LEVEL_HPP_INCLUDE
#define DISH2_DEBUG_LOG_LEVEL_HPP_INCLUDE

#include <limits>

namespace dish2 {

thread_local int log_level{ -1 };

unsigned int log_futex_val{};

unsigned int log_futex_step{ 1 };

constexpr int log_futex_release{ std::numeric_limits<int>::max() };

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_LEVEL_HPP_INCLUDE
