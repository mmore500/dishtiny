#pragma once
#ifndef DISH2_CONFIG_HAS_RNG_PRESEED_HPP_INCLUDE
#define DISH2_CONFIG_HAS_RNG_PRESEED_HPP_INCLUDE

#include <limits>
#include <type_traits>

#include "../config/cfg.hpp"

namespace dish2 {

bool has_rng_preseed() {

  using rng_preseed_t = std::decay_t<decltype( cfg.RNG_PRESEED() )>;
  return cfg.RNG_PRESEED() != std::numeric_limits<rng_preseed_t>::max();

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_HAS_RNG_PRESEED_HPP_INCLUDE
