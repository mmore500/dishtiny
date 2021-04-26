#pragma once
#ifndef DISH2_CONFIG_GET_ENDEAVOR_HPP_INCLUDE
#define DISH2_CONFIG_GET_ENDEAVOR_HPP_INCLUDE

#include <cstdlib>
#include <limits>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../config/cfg.hpp"

#include "has_series.hpp"

namespace dish2 {

emp::optional<uint32_t> get_endeavor() {

  if ( dish2::has_series() ) return cfg.SERIES() / 1000;
  else if ( std::getenv("DISHTINY_ENDEAVOR") )
    return uitsl::stoszt( std::getenv("DISHTINY_ENDEAVOR") );
  else return std::nullopt;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_GET_ENDEAVOR_HPP_INCLUDE
