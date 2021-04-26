#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"



TEST_CASE("Test MpiGuard") {

  // TODO flesh out stub test
  MPI_Barrier(MPI_COMM_WORLD);
  REQUIRE(true);

}
