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
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../utility/sha256_reduce.hpp"

namespace dish2 {

void setup_thread_local_random() {

  const uint32_t preseed = dish2::calc_rng_preseed();

  dish2::log_msg( "using rng preseed ", preseed );

  const uint32_t hash = dish2::sha256_reduce( emp::vector< uint32_t >{
    preseed,
    uitsl::safe_cast<uint32_t>( dish2::thread_idx ),
    uitsl::safe_cast<uint32_t>( uitsl::get_proc_id() )
  } );

  dish2::log_msg( "calculated hash ", hash );

  // seed >= 1 so that rng is seeded deterministically
  const auto seed_addend = hash % ( std::numeric_limits<int32_t>::max() - 1 );
  const int32_t seed = uitsl::safe_cast<int32_t>( 1 + seed_addend );

  sgpl::tlrand.Initialize( seed );

  dish2::log_msg( "applied rng seed ", seed );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_SETUP_THREAD_LOCAL_RANDOM_HPP_INCLUDE
