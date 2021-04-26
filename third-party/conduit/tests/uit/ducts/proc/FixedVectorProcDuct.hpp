#include <ratio>
#include <set>
#include <thread>

#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "netuit/assign/AssignAvailableProcs.hpp"
#include "uitsl/distributed/RdmaWindowManager.hpp"
#include "uitsl/math/math_utils.hpp"
#include "uitsl/mpi/mpi_utils.hpp"
#include "uitsl/nonce/CircularIndex.hpp"

#include "uit/fixtures/Sink.hpp"
#include "uit/fixtures/Source.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/ProConTopologyFactory.hpp"
#include "netuit/arrange/RingTopologyFactory.hpp"
#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNodeInput.hpp"
#include "netuit/mesh/MeshNodeOutput.hpp"

#ifndef TAGS
#define TAGS ""
#endif

using MSG_T = emp::vector<int>;
using Spec = uit::ImplSpec<MSG_T, ImplSel>;

#define REPEAT for (size_t rep = 0; rep < std::deca::num; ++rep)

int tag{};

TEST_CASE("Is initial FixedVectorProcDuct Get() result value-intialized?", "[FixedVectorProcDuct]" TAGS) { REPEAT {

  std::shared_ptr<Spec::ProcBackEnd> backend{
    std::make_shared<Spec::ProcBackEnd>(2)
  };

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ProcOutletDuct>{},
    uit::InterProcAddress{
      uitsl::get_rank(), // outlet proc
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
      ), // inlet proc
      0, // outlet thread
      0, // inlet thread
      tag // tag
    },
    backend
  };

  // corresponding sink
  uit::Sink<Spec>{
    std::in_place_type_t<Spec::ProcInletDuct>{},
    uit::InterProcAddress{
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), 1)
      ), // outlet proc
      uitsl::get_rank(), // inlet proc
      0, // outlet thread
      0, // inlet thread
      tag++ // tag
    },
    backend
  };

  backend->Initialize();

  REQUIRE( outlet.Get() == MSG_T{0, 0} );
  REQUIRE( outlet.JumpGet() == MSG_T{0, 0} );

} }

TEST_CASE("Is transmission reliable?", TAGS) { REPEAT {

  std::shared_ptr<Spec::ProcBackEnd> backend{
    std::make_shared<Spec::ProcBackEnd>(1)
  };

  auto [outlet] = uit::Source<Spec>{
    std::in_place_type_t<Spec::ProcOutletDuct>{},
    uit::InterProcAddress{
      uitsl::get_rank(), // outlet proc
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), -1)
      ), // inlet proc
      0, // outlet thread
      0, // inlet thread
      tag // tag
    },
    backend
  };

  // corresponding sink
  auto [inlet] = uit::Sink<Spec>{
    std::in_place_type_t<Spec::ProcInletDuct>{},
    uit::InterProcAddress{
      uitsl::safe_cast<int>(
        uitsl::circular_index(uitsl::get_rank(), uitsl::get_nprocs(), 1)
      ), // outlet proc
      uitsl::get_rank(), // inlet proc
      0,
      0,
      tag++ // tag
    },
    backend
  };

  backend->Initialize();

  inlet.Put( {10} );
  inlet.Put( {20} );
  inlet.Put( {30} );

  for (int i = 0; i < 1000; ++i) {
    outlet.JumpGet();
  }

  while (  outlet.JumpGet() != MSG_T{30} ) REQUIRE(
    std::set<MSG_T>{{0}, {10}, {20}}.count(outlet.Get())
  );

  REQUIRE( outlet.Get() == MSG_T{30} );

  UITSL_Barrier( MPI_COMM_WORLD );

} }
