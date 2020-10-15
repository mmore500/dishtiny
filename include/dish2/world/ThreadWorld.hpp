#pragma once
#ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
#define DISH2_WORLD_THREADWORLD_HPP_INCLUDE

#include <set>

#include "../../../third-party/conduit/include/netuit/mesh/Mesh.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"

#include "../cell/Cell.hpp"
#include "../config/Cfg.hpp"

namespace dish2 {

template<typename Spec>
struct ThreadWorld {

  emp::vector< dish2::Cell<Spec> > population;

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_submesh_t
    = typename netuit::Mesh<genome_mesh_spec_t>::submesh_t;
  using genome_node_t = netuit::MeshNode<genome_mesh_spec_t>;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_submesh_t
    = typename netuit::Mesh<message_mesh_spec_t>::submesh_t;
  using message_node_t = netuit::MeshNode<message_mesh_spec_t>;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_submesh_t
    = typename netuit::Mesh<resource_mesh_spec_t>::submesh_t;
  using resource_node_t = netuit::MeshNode<resource_mesh_spec_t>;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_submesh_t
    = typename netuit::Mesh<state_mesh_spec_t>::submesh_t;
  using state_node_t = netuit::MeshNode<state_mesh_spec_t>;

  explicit ThreadWorld(
    const genome_submesh_t& genome_submesh,
    const message_submesh_t& message_submesh,
    const resource_submesh_t& resource_submesh,
    const state_submesh_t& state_submesh
  ) {

    emp_assert(( 1 == std::set<size_t>{
      genome_submesh.size(),
      message_submesh.size(),
      resource_submesh.size(),
      state_submesh.size()
    }.size() ));

    for (size_t i{}; i < message_submesh.size(); ++i) {
      population.emplace_back(
        genome_submesh[i],
        message_submesh[i],
        resource_submesh[i],
        state_submesh[i]
      );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
