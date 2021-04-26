#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/mpi/group_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"


TEST_CASE("make_group") {

  if (uitsl::get_rank()) {
    const MPI_Group dyad{
      uitsl::make_group({0, uitsl::get_rank()})
    };
    uitsl::group_to_comm(dyad);
  } else {

    for (uitsl::proc_id_t target = 1; target < uitsl::get_nprocs(); ++target) {
      const MPI_Group dyad{
        uitsl::make_group({0, target})
      };
      uitsl::group_to_comm(dyad);
    }

  }

}
