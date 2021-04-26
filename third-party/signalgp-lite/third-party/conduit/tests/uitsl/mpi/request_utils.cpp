#include <unordered_set>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/math/math_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/mpi/Request.hpp"
#include "uitsl/utility/assign_utils.hpp"


TEST_CASE("test_null") {

  uitsl::Request req;

  REQUIRE( uitsl::test_null(req) );

  char buf;
  UITSL_Irecv(
    &buf, // void *buf
    1, // int count
    MPI_BYTE, // MPI_Datatype datatype
    MPI_ANY_SOURCE, // int source
    MPI_ANY_TAG, // int tag
    MPI_COMM_WORLD, // MPI_Comm comm
    &req // MPI_Request *request
  );

  REQUIRE( !uitsl::test_null(req) );

  UITSL_Cancel(&req);
  UITSL_Request_free(&req);

  REQUIRE( uitsl::test_null(req) );

}
