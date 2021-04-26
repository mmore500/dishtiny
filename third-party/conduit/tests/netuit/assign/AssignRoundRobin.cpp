#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/assign/AssignRoundRobin.hpp"

// TODO: stub test
TEST_CASE("Test AssignRoundRobin") {
  netuit::AssignRoundRobin<size_t>(1)(0);
}
