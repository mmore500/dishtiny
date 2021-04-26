#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignContiguously.hpp"

// TODO: stub test
TEST_CASE("Test AssignContiguously") {
  netuit::AssignContiguously<size_t>(1, 1)(1);
}
