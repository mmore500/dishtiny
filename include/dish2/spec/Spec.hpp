#pragma once
#ifndef DISH2_SPEC_SPEC_HPP_INCLUDE
#define DISH2_SPEC_SPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/topology/RingTopologyFactory.hpp"
#include "../../../third-party/conduit/include/uitsl/distributed/assign_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/assign_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/config/Spec.hpp"

#include "../peripheral/Peripheral.hpp"

#include "GenomeMeshSpec.hpp"
#include "MessageMeshSpec.hpp"
#include "ResourceMeshSpec.hpp"
#include "StateMeshSpec.hpp"

namespace dish2 {

struct Spec {

  using sgpl_spec_t = sgpl::Spec<
    sgpl::CompleteOpLibrary,
    dish2::Peripheral
  >;

  using genome_mesh_spec_t = dish2::GenomeMeshSpec<Spec>;
  using message_mesh_spec_t = dish2::MessageMeshSpec;
  using resource_mesh_spec_t = dish2::ResourceMeshSpec;
  using state_mesh_spec_t = dish2::StateMeshSpec;

  using topology_factory_t = netuit::RingTopologyFactory;

  using thread_assigner_t = uitsl::AssignRoundRobin<uitsl::thread_id_t>;

  using proc_assigner_t = uitsl::AssignRoundRobin<uitsl::proc_id_t>;

};


} // namespace dish2

#endif // #ifndef DISH2_SPEC_SPEC_HPP_INCLUDE
