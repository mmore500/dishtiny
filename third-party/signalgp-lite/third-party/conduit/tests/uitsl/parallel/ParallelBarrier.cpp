#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/countdown/Timer.hpp"
#include "uitsl/parallel/ParallelBarrier.hpp"

TEST_CASE("ParallelBarrier") {

  // TODO expand stub test
  uitsl::ParallelBarrier{2}.ArriveAndWaitWhile(
    uitsl::CoarseTimer{ std::chrono::seconds{ 1 } }
  );
  uitsl::ParallelBarrier{1}.ArriveAndWait();

}
