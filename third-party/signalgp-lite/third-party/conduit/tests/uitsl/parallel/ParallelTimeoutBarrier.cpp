#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/parallel/ParallelTimeoutBarrier.hpp"
#include "uitsl/parallel/ThreadIbarrierFactory.hpp"

TEST_CASE("ParallelTimeoutBarrier") {

  // TODO expand stub test
  const size_t num_threads{ 1 };
  static uitsl::ThreadIbarrierFactory factory{ num_threads };
  uitsl::ParallelTimeoutBarrier{ factory.MakeBarrier() };

}
