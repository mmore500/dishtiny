#pragma once
#ifndef DISH2_DEBUG_BACKTRACE_ENABLE_HPP_INCLUDE
#define DISH2_DEBUG_BACKTRACE_ENABLE_HPP_INCLUDE

#define BACKWARD_HAS_DW 1

#include <stdlib.h>

#include "../../../third-party/backward-cpp/backward.hpp"
#include "../../../third-party/conduit/include/uitsl/nonce/ScopeGuard.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

namespace dish2 {
namespace internal {

const uitsl::ScopeGuard source_path_export_guard(
  [](){ setenv(
    "BACKWARD_CXX_SOURCE_PREFIXES",
    EMP_STRINGIFY(DISHTINY_SOURCE_DIR_),
    0 // won't overwrite
  ); }
);

const backward::SignalHandling backtrace_handler;

} // namespace internal
} // namespace dish2

#endif // #ifndef DISH2_DEBUG_BACKTRACE_ENABLE_HPP_INCLUDE
