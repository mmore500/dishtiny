#pragma once
#ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
#define DISH2_WORLD_PROCWORLD_HPP_INCLUDE

#include <cmath>

#include "../../../third-party/conduit/include/netuit/assign/AssignIntegrated.hpp"
#include "../../../third-party/conduit/include/netuit/assign/GenerateMetisAssignments.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/mpi_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../config/cfg.hpp"
#include "../config/num_cells_global.hpp"

#include "ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
struct ProcWorld {

  using topology_factory_t = typename Spec::topology_factory_t;

  const double dim_width {
    std::pow( dish2::num_cells_global(), 1.0 / dish2::cfg.N_DIMS() )
  };
  const emp::vector< size_t > dims =
    emp::vector< size_t >( dish2::cfg.N_DIMS(), dim_width );

  const netuit::Topology topology{ topology_factory_t{}( dims ) };

  #ifndef __EMSCRIPTEN__
  const std::pair<
    uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::proc_id_t>,
    uitsl::EnumeratedFunctor<netuit::Topology::node_id_t, uitsl::thread_id_t>
  > assignments{ netuit::GenerateMetisAssignments(
    uitsl::audit_cast<size_t>( uitsl::get_nprocs() ),
    dish2::cfg.N_THREADS(),
    topology
  ) };
  #else
  const std::pair<
    uitsl::AssignIntegrated<uitsl::proc_id_t>,
    uitsl::AssignIntegrated<uitsl::thread_id_t>
  > assignments;
  #endif

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  netuit::Mesh<genome_mesh_spec_t> genome_mesh{
    topology,
    assignments.second,
    assignments.first
  };

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  netuit::Mesh<message_mesh_spec_t> message_mesh{
    topology,
    assignments.second,
    assignments.first
  };

  using quorum_mesh_spec_t = typename Spec::quorum_mesh_spec_t;
  netuit::Mesh<quorum_mesh_spec_t> quorum_mesh{
    topology,
    assignments.second,
    assignments.first
  };

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  netuit::Mesh<resource_mesh_spec_t> resource_mesh{
    topology,
    assignments.second,
    assignments.first
  };

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  netuit::Mesh<state_mesh_spec_t> state_mesh{
    topology,
    assignments.second,
    assignments.first
  };

  dish2::ThreadWorld<Spec> MakeThreadWorld(const uitsl::thread_id_t thread_id) {
    return dish2::ThreadWorld<Spec>(
      genome_mesh.GetSubmesh( thread_id ),
      message_mesh.GetSubmesh( thread_id ),
      quorum_mesh.GetSubmesh( thread_id ),
      resource_mesh.GetSubmesh( thread_id ),
      state_mesh.GetSubmesh( thread_id )
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
