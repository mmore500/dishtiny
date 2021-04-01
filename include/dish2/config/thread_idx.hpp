#pragma once
#ifndef DISH2_CONFIG_THREAD_IDX_HPP_INCLUDE
#define DISH2_CONFIG_THREAD_IDX_HPP_INCLUDE

#include <limits>

namespace dish2 {

thread_local uint32_t thread_idx{ std::numeric_limits<uint32_t>::max() };

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_THREAD_IDX_HPP_INCLUDE
