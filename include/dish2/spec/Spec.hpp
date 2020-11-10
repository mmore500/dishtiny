#pragma once
#ifndef DISH2_SPEC_SPEC_HPP_INCLUDE
#define DISH2_SPEC_SPEC_HPP_INCLUDE

#include <tuple>

#include "../../../third-party/conduit/include/netuit/arrange/ToroidalTopologyFactory.hpp"
#include "../../../third-party/conduit/include/netuit/arrange/CompleteTopologyFactory.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/config/Spec.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Core.hpp"

#include "../events/_index.hpp"
#include "../events/EventManager.hpp"
#include "../events/EventSeries.hpp"
#include "../operations/OpLibrary.hpp"
#include "../peripheral/Peripheral.hpp"

#include "_NLEV.hpp"
#include "GenomeMeshSpec.hpp"
#include "IntraMessageMeshSpec.hpp"
#include "MessageMeshSpec.hpp"
#include "QuorumMeshSpec.hpp"
#include "ResourceMeshSpec.hpp"
#include "StateMeshSpec.hpp"

namespace dish2 {

struct Spec {

  constexpr inline static size_t NLEV{ dish2::internal::NLEV };

  // todo implement
  constexpr inline static size_t AMT_NOP_MEMORY = 4;

  using sgpl_spec_t = sgpl::Spec<
    dish2::OpLibrary<dish2::Spec>,
    dish2::Peripheral<dish2::Spec>
  >;

  using event_manager_t = dish2::EventManager<
    dish2::EventSeries< dish2::AlwaysEvent<Spec> >,
    dish2::EventSeries< dish2::KinGroupMatchEvent<Spec>, NLEV - 1 >,
    dish2::EventSeries< dish2::KinGroupMismatchEvent<Spec>, NLEV - 1 >
  >;

  constexpr inline static size_t NUM_EVENTS = event_manager_t::GetNumEvents();

  using tag_t = typename sgpl_spec_t::tag_t;

  using message_t = std::tuple<
    typename sgpl_spec_t::tag_t,
    typename sgpl::Core< sgpl_spec_t >::registers_t
  >;

  using genome_mesh_spec_t = dish2::GenomeMeshSpec<dish2::Spec>;
  using intra_message_mesh_spec_t = dish2::IntraMessageMeshSpec< dish2::Spec >;
  using message_mesh_spec_t = dish2::MessageMeshSpec< dish2::Spec >;
  using quorum_mesh_spec_t = dish2::QuorumMeshSpec<dish2::Spec>;
  using resource_mesh_spec_t = dish2::ResourceMeshSpec;
  using state_mesh_spec_t = dish2::StateMeshSpec<dish2::Spec>;

  using topology_factory_t = netuit::ToroidalTopologyFactory;
  using intra_topology_factory_t = netuit::CompleteTopologyFactory;

};


} // namespace dish2

#endif // #ifndef DISH2_SPEC_SPEC_HPP_INCLUDE
