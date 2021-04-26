#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/Request.hpp"


TEST_CASE("Test Request") {

  uitsl::Request request;
  REQUIRE( request == MPI_REQUEST_NULL );

}
