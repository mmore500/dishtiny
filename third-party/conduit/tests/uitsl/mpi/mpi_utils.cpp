#include <unordered_set>

#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/math/math_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/mpi/Request.hpp"
#include "uitsl/utility/assign_utils.hpp"


TEST_CASE("Test MPI Utils") {
  SECTION("get_nprocs") {

    // TODO
    // int get_nprocs(MPI_Comm comm=MPI_COMM_WORLD)

  }

  SECTION("get_rank and get_proc_id") {

    // TODO
    // proc_id_t get_rank(MPI_Comm comm=MPI_COMM_WORLD)

    // TODO
    // proc_id_t get_proc_id(MPI_Comm comm=MPI_COMM_WORLD)

  }

  SECTION("is_root") {

    // TODO
    // bool is_root(MPI_Comm comm=MPI_COMM_WORLD)

  }

  SECTION("is_multiprocess") {

    // TODO
    // bool is_multiprocess()

  }

  SECTION("comm_to_group") {

    // TODO
    // MPI_Group comm_to_group(const MPI_Comm & comm)

  }

  SECTION("group_to_comm") {

    // TODO
    // MPI_Comm group_to_comm(
    //   const MPI_Group & group,
    //   const MPI_Comm & comm=MPI_COMM_WORLD
    // )

  }

  SECTION("intersect_groups") {

    const int my_rank{ uitsl::get_rank() };

    const int num_ranks{ uitsl::get_nprocs() };

    const MPI_Group every_other{ uitsl::comm_to_group(
      uitsl::split_comm(uitsl::AssignRoundRobin<uitsl::proc_id_t>{2})
    ) };
    const MPI_Group halves{ uitsl::comm_to_group(
      uitsl::split_comm(uitsl::AssignContiguously<uitsl::proc_id_t>{
        2,
        uitsl::safe_cast<size_t>(num_ranks)
      })
    ) };
    const MPI_Group quarters{ uitsl::comm_to_group(
      uitsl::split_comm(uitsl::AssignContiguously<uitsl::proc_id_t>{
        4,
        uitsl::safe_cast<size_t>(num_ranks)
      })
    ) };

    REQUIRE(
      uitsl::group_size(uitsl::intersect_groups({halves, quarters}))
        == uitsl::group_size(quarters)
    );

    if (num_ranks >= 4) REQUIRE(
      uitsl::group_size(uitsl::intersect_groups({halves, every_other}))
        < uitsl::group_size(halves)
    );

    if (num_ranks >= 4) REQUIRE(
      uitsl::group_size(uitsl::intersect_groups({halves, every_other}))
        < uitsl::group_size(every_other)
    );

    const emp::vector<uitsl::proc_id_t> halves_quarters_ranks(
      uitsl::get_group_ranks(uitsl::intersect_groups({halves, quarters}))
    );

    REQUIRE( std::all_of(
      std::begin(halves_quarters_ranks),
      std::end(halves_quarters_ranks),
      [=](const auto & rank){
        return uitsl::AssignContiguously<uitsl::proc_id_t>{
          2,
          uitsl::safe_cast<size_t>(num_ranks)
        }(rank) == uitsl::AssignContiguously<uitsl::proc_id_t>{
          2,
          uitsl::safe_cast<size_t>(num_ranks)
        }(my_rank);
      }
    ) );

    REQUIRE( std::all_of(
      std::begin(halves_quarters_ranks),
      std::end(halves_quarters_ranks),
      [=](const auto & rank){
        return uitsl::AssignContiguously<uitsl::proc_id_t>{
          4,
          uitsl::safe_cast<size_t>(num_ranks)
        }(rank) == uitsl::AssignContiguously<uitsl::proc_id_t>{
          4,
          uitsl::safe_cast<size_t>(num_ranks)
        }(my_rank);
      }
    ) );

    const emp::vector<uitsl::proc_id_t> halves_every_other_ranks(
      uitsl::get_group_ranks(uitsl::intersect_groups({every_other, halves}))
    );

    REQUIRE( std::all_of(
      std::begin(halves_every_other_ranks),
      std::end(halves_every_other_ranks),
      [=](const auto & rank){
        return uitsl::AssignRoundRobin<uitsl::proc_id_t>{2}(rank) == uitsl::AssignRoundRobin<uitsl::proc_id_t>{2}(my_rank);
      }
    ) );

    REQUIRE( std::all_of(
      std::begin(halves_every_other_ranks),
      std::end(halves_every_other_ranks),
      [=](const auto & rank){
        return uitsl::AssignContiguously<uitsl::proc_id_t>{
          2,
          uitsl::safe_cast<size_t>(num_ranks)
        }(rank) == uitsl::AssignContiguously<uitsl::proc_id_t>{
          2,
          uitsl::safe_cast<size_t>(num_ranks)
        }(my_rank);
      }
    ) );


  }

  SECTION("combine_groups") {

    const uitsl::proc_id_t my_rank{ uitsl::get_rank() };

    const uitsl::proc_id_t num_ranks{ uitsl::get_nprocs() };

    const MPI_Group every_other{ uitsl::comm_to_group(
      uitsl::split_comm(uitsl::AssignRoundRobin<uitsl::proc_id_t>{2})
    ) };
    const MPI_Group halves{ uitsl::comm_to_group(
      uitsl::split_comm(uitsl::AssignContiguously<uitsl::proc_id_t>{
        2,
        uitsl::safe_cast<size_t>(num_ranks)
      })
    ) };
    const MPI_Group quarters{ uitsl::comm_to_group(
      uitsl::split_comm(uitsl::AssignContiguously<uitsl::proc_id_t>{
        4,
        uitsl::safe_cast<size_t>(num_ranks)
      })
    ) };

    REQUIRE(
      uitsl::group_size(uitsl::combine_groups({halves, quarters}))
        == uitsl::group_size(halves)
    );

    if (num_ranks >= 4) REQUIRE(
      uitsl::group_size(uitsl::combine_groups({halves, every_other}))
        > uitsl::group_size(halves)
    );

    const emp::vector<uitsl::proc_id_t> group_ranks(uitsl::get_group_ranks(
      uitsl::combine_groups({halves, every_other})
    ) );

    if (num_ranks >= 4) REQUIRE( std::any_of(
      std::begin(group_ranks),
      std::end(group_ranks),
      [my_rank](const auto & rank){
        return uitsl::AssignRoundRobin<uitsl::proc_id_t>{2}(rank) != uitsl::AssignRoundRobin<uitsl::proc_id_t>{2}(my_rank);
      }
    ) );

    if (num_ranks >= 4) REQUIRE( std::any_of(
      std::begin(group_ranks),
      std::end(group_ranks),
      [my_rank](const auto & rank){
        return uitsl::AssignRoundRobin<uitsl::proc_id_t>{2}(rank) == uitsl::AssignRoundRobin<uitsl::proc_id_t>{2}(my_rank);
      }
    ) );

  }

  SECTION("subtract_groups") {

    // TODO
    // MPI_Group subtract_groups(
    //   const MPI_Group & minuend,
    //   const MPI_Group & subtrahend
    // )

  }

  SECTION("group_size") {

    // TODO
    // size_t group_size(const MPI_Group & group)

  }

  SECTION("comm_size") {

    // TODO
    // size_t comm_size(const MPI_Comm & comm)

  }

  SECTION("split_comm") {

    const uitsl::proc_id_t my_rank{ uitsl::get_rank() };

    const uitsl::proc_id_t num_ranks{ uitsl::get_nprocs() };

    // TODO refactor to use assign_utils
    const MPI_Comm every_other{
      uitsl::split_comm([](const uitsl::proc_id_t rank){ return rank%2 == 0; })
    };
    const MPI_Comm halves{
      uitsl::split_comm([=](const uitsl::proc_id_t rank){
        return rank * 2 / num_ranks;
      })
    };

    REQUIRE(
      uitsl::difference(uitsl::comm_size(every_other), uitsl::comm_size(halves)) <= 1
    );

    emp::vector<uitsl::proc_id_t> comm_ranks( uitsl::comm_size(every_other) );

    UITSL_Allgather(
      &my_rank, // const void *sendbuf
      1, // int sendcount
      MPI_INT, // MPI_Datatype sendtype
      comm_ranks.data(), // void *recvbuf
      1, // int recvcount
      MPI_INT, // MPI_Datatype recvtype
      every_other // MPI_Comm comm
    );

    REQUIRE( std::all_of(
      std::begin(comm_ranks),
      std::end(comm_ranks),
      [my_rank](const auto & rank){ return rank % 2 == my_rank % 2; }
    ) );

    comm_ranks.resize( uitsl::comm_size(halves) );

    UITSL_Allgather(
      &my_rank, // const void *sendbuf
      1, // int sendcount
      MPI_INT, // MPI_Datatype sendtype
      comm_ranks.data(), // void *recvbuf
      1, // int recvcount
      MPI_INT, // MPI_Datatype recvtype
      halves // MPI_Comm comm
    );

    REQUIRE( std::all_of(
      std::begin(comm_ranks),
      std::end(comm_ranks),
      [=](const auto & rank){
        return rank * 2 / num_ranks == my_rank * 2 / num_ranks;
      }
    ) );

  }

  SECTION("comm_to_string") {

    REQUIRE(!uitsl::comm_to_string(MPI_COMM_WORLD).empty());

  }

  SECTION("test_null") {

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
}
