#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNRANDOMLY_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNRANDOMLY_HPP_INCLUDE

#include <functional>
#include <stddef.h>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

namespace netuit {

template<typename RETURN_TYPE>
struct AssignRandomly {
  const size_t num_threads;
  emp::Random& rand;
  emp::vector<RETURN_TYPE> ids;

  AssignRandomly(
    const size_t num_threads_
  , emp::Random& rand_
  ) : num_threads(num_threads_)
    , rand(rand_)
    , ids(emp::GetPermutation(rand, num_threads))
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return ids[node_id % num_threads];
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNRANDOMLY_HPP_INCLUDE
