#pragma once
#ifndef DISH2_INTROSPECTION_GET_METASERIES_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_METASERIES_HPP_INCLUDE

#include <cstdlib>
#include <limits>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

emp::optional<size_t> get_endeavor() {

  if ( cfg.SERIES() != std::numeric_limits<size_t>::max() )
    return cfg.SERIES() % 1000;
  else if ( std::getenv("DISHTINY_ENDEAVOR") )
    return uitsl::stoszt( std::getenv("DISHTINY_ENDEAVOR") );
  else return std::nullopt;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_METASERIES_HPP_INCLUDE
