#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

#include "netuit/assign/AssignRandomly.hpp"

// TODO: stub test
TEST_CASE("Test AssignRandomly") {
  emp::Random rand;
  netuit::AssignRandomly<size_t>(1, rand)(0);
}
