#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/mpi_types.hpp"

TEST_CASE("byte") { REQUIRE(uitsl::datatype_from_type<std::byte>() == MPI_BYTE); }

TEST_CASE("int") { REQUIRE(uitsl::datatype_from_type<int>() == MPI_INT); }

TEST_CASE("double") {
  REQUIRE(uitsl::datatype_from_type<double>() == MPI_DOUBLE);
}
