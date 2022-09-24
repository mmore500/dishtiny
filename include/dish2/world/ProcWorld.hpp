#pragma once
#ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
#define DISH2_WORLD_PROCWORLD_HPP_INCLUDE

#include <cmath>
#include <functional>
#include <iostream>
#include <utility>

#include "../../../third-party/conduit/include/netuit/assign/AssignIntegrated.hpp"
#include "../../../third-party/conduit/include/netuit/assign/AssignPerfectHypercube.hpp"
#include "../../../third-party/conduit/include/netuit/assign/GenerateMetisAssignments.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/safe_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/math/is_perfect_hypercube.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/nonce/ThreadUidNormalizer.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../config/cfg.hpp"
#include "../config/num_cells_global.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"

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
  const size_t total_threads = uitsl::get_nprocs() * dish2::cfg.N_THREADS();
  const bool use_metis = !(
    (dish2::num_cells_global() % uitsl::get_nprocs() == 0)
    && (dish2::num_cells_global() % total_threads == 0)
    && uitsl::is_perfect_hypercube(
      dish2::num_cells_global(), dish2::cfg.N_DIMS()
    )
    && uitsl::is_perfect_hypercube(
      uitsl::get_nprocs(), dish2::cfg.N_DIMS()
    )
    && uitsl::is_perfect_hypercube(
      total_threads, dish2::cfg.N_DIMS()
    )
  );

  const std::pair<
    std::function<uitsl::proc_id_t(size_t)>,
    std::function<uitsl::thread_id_t(size_t)>
  > assignments = use_metis
  ? netuit::GenerateMetisAssignmentFunctors(
    uitsl::safe_cast<size_t>( uitsl::get_nprocs() ),
    dish2::cfg.N_THREADS(),
    topology
  )
  : std::pair{
    netuit::AssignPerfectHypercube<uitsl::proc_id_t>(
      dish2::cfg.N_DIMS(), dish2::num_cells_global(), uitsl::get_nprocs()
    ),
    uitsl::ThreadUidNormalizer(
      netuit::AssignPerfectHypercube<uitsl::proc_id_t>(
        dish2::cfg.N_DIMS(), dish2::num_cells_global(), uitsl::get_nprocs()
      ),
      netuit::AssignPerfectHypercube<uitsl::thread_id_t>(
        dish2::cfg.N_DIMS(), dish2::num_cells_global(), total_threads
      )
    )
  };
  #else // #ifndef __EMSCRIPTEN__
  const bool use_metis = false;
  const std::pair<
    uitsl::AssignIntegrated<uitsl::proc_id_t>,
    uitsl::AssignIntegrated<uitsl::thread_id_t>
  > assignments;
  #endif // #ifndef __EMSCRIPTEN__

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

  using push_mesh_spec_t = typename Spec::push_mesh_spec_t;
  netuit::Mesh<push_mesh_spec_t> push_mesh{
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

  ProcWorld() { if (use_metis) dish2::log_msg( "assign used metis" ); }

  dish2::ThreadWorld<Spec> MakeThreadWorld() {
    return dish2::ThreadWorld<Spec>(
      genome_mesh.GetSubmesh( dish2::thread_idx ),
      message_mesh.GetSubmesh( dish2::thread_idx ),
      push_mesh.GetSubmesh( dish2::thread_idx ),
      quorum_mesh.GetSubmesh( dish2::thread_idx ),
      resource_mesh.GetSubmesh( dish2::thread_idx ),
      state_mesh.GetSubmesh( dish2::thread_idx )
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
