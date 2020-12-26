#pragma once
#ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
#define DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE

#include <iostream>
#include <limits>

#include "../../../third-party/conduit/include/uitsl/debug/safe_cast.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/meta/meta.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"
#include "../utility/sha256_reduce.hpp"

namespace dish2 {

void setup_thread_local_random( const size_t thread_idx ) {

  const size_t computed_preseed = []{
    return dish2::sha256_reduce( emp::vector< size_t >{
      dish2::cfg.STINT(), dish2::cfg.SERIES(), dish2::cfg.REPLICATE()
    } );
  }();

  const size_t preseed =
    ( dish2::cfg.RNG_PRESEED() != std::numeric_limits<size_t>::max() )
    ? dish2::cfg.RNG_PRESEED()
    : computed_preseed
  ;

  std::cout << "thread " << thread_idx << " using rng preseed " << preseed;
  std::cout  << std::endl;

  const size_t hash = dish2::sha256_reduce( emp::vector< size_t >{
    preseed, thread_idx
  } );

  // seed >= 1 so that rng is seeded deterministically
  const auto seed_addend = hash % ( std::numeric_limits<int>::max() - 1 );
  const int seed = uitsl::safe_cast<int>( 1 + seed_addend );

  sgpl::ThreadLocalRandom::Get() = emp::Random( seed );

  std::cout << "thread " << thread_idx << " applied rng seed " << seed;
  std::cout << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
