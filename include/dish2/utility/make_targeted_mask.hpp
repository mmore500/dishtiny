#pragma once
#ifndef DISH2_UTILITY_MAKE_TARGETED_MASK_HPP_INCLUDE
#define DISH2_UTILITY_MAKE_TARGETED_MASK_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

namespace dish2 {

template< typename Program >
emp::vector<char> make_targeted_mask(
  const Program& program, const size_t num_to_nop, const size_t seed
) {


  emp::vector<size_t> candidate_idxs;
  for (size_t idx{}; idx < program.size(); ++idx) {
    if ( program[idx].IsOp() ) candidate_idxs.push_back( idx );
  }

  emp::Random rand( seed );
  emp::Shuffle( rand, candidate_idxs );

  candidate_idxs.resize(
    std::min( candidate_idxs.size(), num_to_nop )
  );


  emp::vector<char> res( program.size(), false );
  for ( const size_t idx : candidate_idxs ) res[idx] = true;

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_MAKE_TARGETED_MASK_HPP_INCLUDE
