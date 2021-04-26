#include <algorithm>
#include <set>
#include <thread>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/include/emp/base/vector.hpp"

#include "uitsl/concurrent/Gatherer.hpp"
#if 0
#endif
#include "uitsl/debug/safe_cast.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"
#include "uitsl/parallel/ThreadTeam.hpp"
#include "uitsl/parallel/thread_utils.hpp"
#include "uitsl/polyfill/barrier.hpp"


constexpr size_t num_threads{ 2 };

uitsl::Gatherer<int> gather(MPI_INT);

inline void do_work_gatherer() {
  static std::barrier barrier{uitsl::safe_cast<std::ptrdiff_t>(num_threads)};

  barrier.arrive_and_wait();

  gather.Put(uitsl::get_thread_id() + num_threads * uitsl::get_proc_id());

}


TEST_CASE("Test Gatherer") {

  uitsl::ThreadTeam team;
  team.Add(do_work_gatherer);
  team.Add(do_work_gatherer);
  team.Join();

  const auto res = gather.Gather();

  if (uitsl::is_root()) REQUIRE( res );

  if (res) {
    emp::vector<int> target(num_threads * uitsl::get_nprocs());
    std::iota(std::begin(target), std::end(target), 0);

    REQUIRE(
      std::set(std::begin(*res), std::end(*res))
        == std::set(std::begin(target), std::end(target))
    );

  };

}
