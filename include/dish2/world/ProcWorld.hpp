#pragma once
#ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
#define DISH2_WORLD_PROCWORLD_HPP_INCLUDE

#include "../config/Cfg.hpp"

#include "ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
struct ProcWorld {

  using topology_factory_t = typename Spec::topology_factory_t;
  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using thread_assigner_t = typename Spec::thread_assigner_t;
  using proc_assigner_t = typename Spec::proc_assigner_t;

  netuit::Mesh<message_mesh_spec_t> message_mesh{
    topology_factory_t{}( dish2::Cfg::N_CELLS() ),
    thread_assigner_t{ dish2::Cfg::N_CELLS(), dish2::Cfg::N_CELLS() /*FIXME*/ },
    proc_assigner_t{ dish2::Cfg::N_PROCS(), dish2::Cfg::N_CELLS() }
  };

  netuit::Mesh<resource_mesh_spec_t> resource_mesh{
    topology_factory_t{}( dish2::Cfg::N_CELLS() ),
    thread_assigner_t{ dish2::Cfg::N_CELLS(), dish2::Cfg::N_CELLS() /*FIXME*/ },
    proc_assigner_t{ dish2::Cfg::N_PROCS(), dish2::Cfg::N_CELLS() }
  };

  dish2::ThreadWorld<Spec> MakeThreadWorld(const uitsl::thread_id_t thread_id) {
    return dish2::ThreadWorld<Spec>(
      message_mesh.GetSubmesh( thread_id ),
      resource_mesh.GetSubmesh( thread_id )
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_PROCWORLD_HPP_INCLUDE
