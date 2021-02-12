#pragma once
#ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
#define DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE

#include <iostream>
#include <limits>

#include "../../../third-party/conduit/include/uitsl/debug/safe_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/meta/meta.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/calc_rng_preseed.hpp"
#include "../config/cfg.hpp"
#include "../utility/sha256_reduce.hpp"

namespace dish2 {

void setup_thread_local_random( const size_t thread_idx ) {

  const uint32_t preseed = dish2::calc_rng_preseed();

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " using rng preseed " << preseed << std::endl;

  const uint32_t hash = dish2::sha256_reduce( emp::vector< uint32_t >{
    preseed,
    uitsl::safe_cast<uint32_t>( thread_idx ),
    uitsl::safe_cast<uint32_t>( uitsl::get_proc_id() )
  } );

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " calculated hash " << hash << std::endl;

  // seed >= 1 so that rng is seeded deterministically
  const auto seed_addend = hash % ( std::numeric_limits<int32_t>::max() - 1 );
  const int32_t seed = uitsl::safe_cast<int32_t>( 1 + seed_addend );

  sgpl::tlrand.Initialize( seed );

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " applied rng seed " << seed << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
