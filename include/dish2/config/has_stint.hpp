#pragma once
#ifndef DISH2_CONFIG_HAS_STINT_HPP_INCLUDE
#define DISH2_CONFIG_HAS_STINT_HPP_INCLUDE

#include <limits>
#include <type_traits>

#include "../config/cfg.hpp"

namespace dish2 {

bool has_stint() {

  using stint_t = std::decay_t<decltype( cfg.STINT() )>;
  return cfg.STINT() != std::numeric_limits<stint_t>::max();

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_HAS_STINT_HPP_INCLUDE
