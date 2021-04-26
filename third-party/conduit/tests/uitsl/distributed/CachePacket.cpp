#include <string>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/distributed/CachePacket.hpp"


TEST_CASE("Test CachePacket") {

  // TODO flesh out stub test
  uitsl::CachePacket<int>{};
  uitsl::CachePacket<std::string>{};

}
