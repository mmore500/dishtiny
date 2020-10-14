#pragma once
#ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
#define DISH2_WORLD_THREADWORLD_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/Mesh.hpp"

#include "../config/Cfg.hpp"

namespace dish2 {

template<typename Spec>
struct ThreadWorld {

  using uit_resource_spec_t = typename Spec::uit_resource_spec_t;
  using submesh_t = typename netuit::Mesh<uit_resource_spec_t>::submesh_t;

  submesh_t resource_submesh;

  explicit ThreadWorld(const submesh_t& resource_submesh_)
  : resource_submesh(resource_submesh_)
  {}

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
