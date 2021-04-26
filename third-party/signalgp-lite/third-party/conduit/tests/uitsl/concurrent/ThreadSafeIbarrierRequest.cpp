#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/concurrent/ThreadSafeIbarrierRequest.hpp"

#if 0
#endif

#include "uitsl/parallel/ThreadTeam.hpp"


constexpr size_t num_threads{ 2 };

inline void do_work_barrier() {

  const uitsl::ThreadSafeIbarrierRequest barrier;
  while( !barrier.IsComplete() );
  REQUIRE( barrier.IsComplete() );

}


TEST_CASE("Test ThreadSafeIbarrierRequest") {

  uitsl::ThreadTeam team;

  team.Add(do_work_barrier);
  team.Add(do_work_barrier);

  team.Join();

}
