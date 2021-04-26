#pragma once
#ifndef UITSL_DEBUG_WARNONCE_HPP_INCLUDE
#define UITSL_DEBUG_WARNONCE_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/errors.hpp"

namespace uitsl {

class WarnOnce {

public:

  explicit WarnOnce(const std::string& warning) {
    emp::NotifyWarning(warning);
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_WARNONCE_HPP_INCLUDE
