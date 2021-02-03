#pragma once
#ifndef DISH2_CONFIG_HAS_SERIES_HPP_INCLUDE
#define DISH2_CONFIG_HAS_SERIES_HPP_INCLUDE

#include <limits>
#include <type_traits>

#include "../config/cfg.hpp"

namespace dish2 {

bool has_series() {

  using series_t = std::decay_t<decltype( cfg.SERIES() )>;
  return cfg.SERIES() != std::numeric_limits<series_t>::max();

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_HAS_SERIES_HPP_INCLUDE
