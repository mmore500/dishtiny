#pragma once

#include <memory>

#include "../../../third-party/Empirical/include/emp/hardware/EventDrivenGP.hpp"

#include "netuit/mesh/Mesh.hpp"
#include "netuit/mesh/MeshNode.hpp"

#include "config.hpp"

class Trait;

using trait_t = std::unique_ptr<Trait>;

using hardware_t = emp::EventDrivenGP_AW<
  TAG_WIDTH,
  trait_t
>;

using program_t = hardware_t::program_t;

using inst_lib_t = emp::InstLib<hardware_t>;

using event_lib_t = emp::EventLib<hardware_t>;

using event_t = hardware_t::event_t;

using inst_t = hardware_t::inst_t;

using message_t = hardware_t::event_t;

using tag_t = hardware_t::affinity_t;

#include "ImplSpec.hpp"

using submesh_t = typename netuit::Mesh<ImplSpec>::submesh_t;

using node_t = netuit::MeshNode<ImplSpec>;
