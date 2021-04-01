#pragma once
#ifndef DISH2_UTILITY_MAKE_MASK_HPP_INCLUDE
#define DISH2_UTILITY_MAKE_MASK_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

namespace dish2 {

emp::vector<char> make_mask(
  const size_t num_sites, const size_t num_true, const size_t seed
) {

  emp_assert( num_true <= num_sites );

  emp::vector<char> res( num_true, true );
  res.resize( num_sites );

  emp::Random rand( seed );
  emp::Shuffle( rand, res );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_MAKE_MASK_HPP_INCLUDE
