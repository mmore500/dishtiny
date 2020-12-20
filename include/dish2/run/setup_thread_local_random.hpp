#pragma once
#ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
#define DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/debug/safe_cast.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/meta/meta.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

void setup_thread_local_random( const size_t thread_idx ) {

  const auto hash = emp::CombineHash( thread_idx, dish2::cfg.RNG_SEED() );
  const auto seed_addend = hash % ( std::numeric_limits<int>::max() - 1 );
  const int seed = uitsl::safe_cast<int>( 1 + seed_addend );

  sgpl::ThreadLocalRandom::Get() = emp::Random( seed );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
