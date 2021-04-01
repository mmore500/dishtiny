#pragma once
#ifndef DISH2_UTILITY_NOP_NTH_OP_HPP_INCLUDE
#define DISH2_UTILITY_NOP_NTH_OP_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp_lite/include/emp/math/random_utils.hpp"

namespace dish2 {

emp::vector<char> nop_nth_op(
  const auto& program, const size_t num_to_nop, const size_t seed
) {

  emp::vector<size_t> candidate_idxs;
  for (size_t idx{}; idx < program.size(); ++idx) {
    if ( program[idx].IsOp() ) candidate_idxs.push_back( idx );
  }

  emp::Random rand( seed );
  emp::Shuffle( rand, candidate_idxs );


  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_NOP_NTH_OP_HPP_INCLUDE
