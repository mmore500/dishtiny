#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/parallel/RecursiveExclusiveLock.hpp"
#include "uitsl/parallel/RecursiveMutex.hpp"

TEST_CASE("RecursiveExclusiveLock") {

  // TODO expand stub test
  uitsl::RecursiveMutex mutex;
  uitsl::RecursiveExclusiveLock{ mutex };

}
