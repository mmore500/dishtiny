#pragma once
#ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
#define DISH2_WORLD_PROCWORLD_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/debug/audit_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/mpi_utils.hpp"

#include "../config/cfg.hpp"

#include "ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
struct ProcWorld {

  using topology_factory_t = typename Spec::topology_factory_t;
  using thread_assigner_t = typename Spec::thread_assigner_t;
  using proc_assigner_t = typename Spec::proc_assigner_t;

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  netuit::Mesh<genome_mesh_spec_t> genome_mesh{
    topology_factory_t{}( dish2::cfg.N_CELLS() ),
    thread_assigner_t{ dish2::cfg.N_THREADS(), dish2::cfg.N_CELLS() },
    proc_assigner_t{
      uitsl::audit_cast<size_t>( uitsl::get_nprocs() ), dish2::cfg.N_CELLS()
    }
  };

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  netuit::Mesh<message_mesh_spec_t> message_mesh{
    topology_factory_t{}( dish2::cfg.N_CELLS() ),
    thread_assigner_t{ dish2::cfg.N_THREADS(), dish2::cfg.N_CELLS() },
    proc_assigner_t{
      uitsl::audit_cast<size_t>( uitsl::get_nprocs() ), dish2::cfg.N_CELLS()
    }
  };

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  netuit::Mesh<resource_mesh_spec_t> resource_mesh{
    topology_factory_t{}( dish2::cfg.N_CELLS() ),
    thread_assigner_t{ dish2::cfg.N_THREADS(), dish2::cfg.N_CELLS() },
    proc_assigner_t{
      uitsl::audit_cast<size_t>( uitsl::get_nprocs() ), dish2::cfg.N_CELLS()
    }
  };

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  netuit::Mesh<state_mesh_spec_t> state_mesh{
    topology_factory_t{}( dish2::cfg.N_CELLS() ),
    thread_assigner_t{ dish2::cfg.N_THREADS(), dish2::cfg.N_CELLS() },
    proc_assigner_t{
      uitsl::audit_cast<size_t>( uitsl::get_nprocs() ), dish2::cfg.N_CELLS()
    }
  };

  dish2::ThreadWorld<Spec> MakeThreadWorld(const uitsl::thread_id_t thread_id) {
    return dish2::ThreadWorld<Spec>(
      genome_mesh.GetSubmesh( thread_id ),
      message_mesh.GetSubmesh( thread_id ),
      resource_mesh.GetSubmesh( thread_id ),
      state_mesh.GetSubmesh( thread_id )
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
