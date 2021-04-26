#pragma once
#ifndef DISH2_CONFIG_CALC_RNG_PRESEED_HPP_INCLUDE
#define DISH2_CONFIG_CALC_RNG_PRESEED_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../config/cfg.hpp"
#include "../utility/sha256_reduce.hpp"

#include "has_rng_preseed.hpp"

namespace dish2 {

auto calc_rng_preseed() {

  const uint32_t computed_preseed = dish2::sha256_reduce(
    emp::vector< uint32_t >{
      dish2::cfg.STINT(),
      dish2::cfg.SERIES(),
      dish2::sha256_reduce( dish2::cfg.REPLICATE() )
    }
  );

  return dish2::has_rng_preseed()
    ? dish2::cfg.RNG_PRESEED()
    : computed_preseed;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_CALC_RNG_PRESEED_HPP_INCLUDE
