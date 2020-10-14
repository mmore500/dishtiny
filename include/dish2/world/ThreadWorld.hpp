#pragma once
#ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
#define DISH2_WORLD_THREADWORLD_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/Mesh.hpp"

#include "../config/Cfg.hpp"

namespace dish2 {

template<typename Spec>
struct ThreadWorld {

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_submesh_t
    = typename netuit::Mesh<message_mesh_spec_t>::submesh_t;
  message_submesh_t message_submesh;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_submesh_t
    = typename netuit::Mesh<resource_mesh_spec_t>::submesh_t;
  resource_submesh_t resource_submesh;

  explicit ThreadWorld(
    const message_submesh_t& message_submesh_,
    const resource_submesh_t& resource_submesh_
  ) : message_submesh(message_submesh_)
  , resource_submesh(resource_submesh_)
  {}

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
