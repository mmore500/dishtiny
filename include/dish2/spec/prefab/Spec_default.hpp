#pragma once
#ifndef DISH2_SPEC_PREFAB_SPEC_DEFAULT_HPP_INCLUDE
#define DISH2_SPEC_PREFAB_SPEC_DEFAULT_HPP_INCLUDE

#include <tuple>
#include <type_traits>

#include "../../../../third-party/conduit/include/netuit/arrange/CompleteTopologyFactory.hpp"
#include "../../../../third-party/conduit/include/netuit/arrange/ToroidalTopologyFactory.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/hardware/Core.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/library/OpLibraryCoupler.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/library/prefab/CompleteOpLibrary.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/program/Program.hpp"
#include "../../../../third-party/signalgp-lite/include/sgpl/spec/Spec.hpp"

#include "../../events/EventManager.hpp"
#include "../../events/EventSeries.hpp"
#include "../../events/_index.hpp"
#include "../../operations/_index.hpp"
#include "../../peripheral/Peripheral.hpp"
#include "../../push/PushMessage.hpp"
#include "../../services/_index.hpp"
#include "../../services/ServiceManager.hpp"
#include "../../services_threadlocal/_index.hpp"
#include "../../services_threadlocal/ThreadLocalServiceManager.hpp"

#include "../GenomeMeshSpec.hpp"
#include "../IntraMessageMeshSpec.hpp"
#include "../MessageMeshSpec.hpp"
#include "../_NLEV.hpp"
#include "../PushMeshSpec.hpp"
#include "../QuorumMeshSpec.hpp"
#include "../ResourceMeshSpec.hpp"
#include "../StateMeshSpec.hpp"

namespace dish2 {

struct Spec_default {

  using this_t = Spec_default;

  /// How many hierarchical kin group levels should be simulated?
  constexpr inline static size_t NLEV{ DISH2_NLEV };

  /// How many nop and transient nop states should exist in the peripheral?
  constexpr inline static size_t AMT_NOP_MEMORY = 4;

  /// How many callees should we displace state by in state exchange
  /// experiments?
  constexpr inline static size_t STATE_EXCHANGE_CHAIN_LENGTH = 128;

  /// Should higher-level kin group quorum counts be based on cell count or sub-
  /// kin group count?
  constexpr inline static bool SET_QUORUM_BITS_BY_KIN_GROUP = true;

  using op_library_t = sgpl::OpLibraryCoupler<
    sgpl::CompleteOpLibrary,
    sgpl::global::RegulatorAdj<1>, // exposed regulator
    sgpl::global::RegulatorGet<1>, // exposed regulator
    sgpl::global::RegulatorSet<1>, // exposed regulator
    sgpl::global::RegulatorDecay<1>, // exposed regulator
    dish2::AddToOwnState< this_t >,
    dish2::BcstIntraMessageIf,
    dish2::MultiplyOwnState< this_t >,
    dish2::ReadNeighborState< this_t >,
    dish2::ReadOwnState< this_t >,
    dish2::SendInterMessageIf,
    dish2::SendIntraMessageIf,
    dish2::WriteOwnStateIf< this_t >
  >;

  struct sgpl_spec_t : public sgpl::Spec<
    op_library_t,
    dish2::Peripheral<this_t>
  > {
    /// How many virtual cores should each cardinal's virtual CPU be able to
    /// support?
    static constexpr inline size_t num_cores{ 32 };
  };

  using event_manager_t = dish2::EventManager<
    dish2::EventSeries< dish2::AlwaysEvent >,
    dish2::EventSeries< dish2::IsChildCellOfEvent >,
    dish2::EventSeries< dish2::IsChildGroupOfEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::IsNewbornEvent >,
    dish2::EventSeries< dish2::IsParentCellOfEvent >,
    dish2::EventSeries< dish2::KinGroupMatchEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::KinGroupMismatchEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::KinGroupWillExpireEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::KinGroupWillNotExpireEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::NeighborApoptosisEvent >,
    dish2::EventSeries< dish2::NeighborFragmentedEvent >,
    dish2::EventSeries< dish2::NeighborIsAliveEvent >,
    dish2::EventSeries< dish2::NeighborIsNewbornEvent >,
    dish2::EventSeries< dish2::NeighborIsNotAliveEvent >,
    dish2::EventSeries< dish2::NeighborKinGroupWillExpireEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::NeighborOptimumQuorumExceededEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::OptimumQuorumExceededEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::OptimumQuorumNotExceededEvent, NLEV - 1 >,
    dish2::EventSeries< dish2::ParentFragmentedEvent >,
    dish2::EventSeries< dish2::PhylogeneticRootMatchEvent >,
    dish2::EventSeries< dish2::PhylogeneticRootMismatchEvent >,
    dish2::EventSeries< dish2::PoorerThanNeighborEvent >,
    dish2::EventSeries< dish2::ReceivedResourceFromEvent >,
    dish2::EventSeries< dish2::RicherThanNeighborEvent >,
    dish2::EventSeries< dish2::StockpileDepletedEvent >,
    dish2::EventSeries< dish2::StockpileFecundEvent >
  >;

  constexpr inline static size_t NUM_EVENTS = event_manager_t::GetNumEvents();

  using tag_t = typename sgpl_spec_t::tag_t;

  using message_t = std::tuple<
    typename sgpl_spec_t::tag_t,
    typename sgpl::Core< sgpl_spec_t >::registers_t
  >;

  using push_message_t = dish2::PushMessage;

  using genome_mesh_spec_t = dish2::GenomeMeshSpec<this_t>;
  using intra_message_mesh_spec_t = dish2::IntraMessageMeshSpec< this_t >;
  using message_mesh_spec_t = dish2::MessageMeshSpec< this_t >;
  using push_mesh_spec_t = dish2::PushMeshSpec<this_t>;
  using quorum_mesh_spec_t = dish2::QuorumMeshSpec<this_t>;
  using resource_mesh_spec_t = dish2::ResourceMeshSpec;
  using state_mesh_spec_t = dish2::StateMeshSpec<this_t>;

  using topology_factory_t = netuit::ToroidalTopologyFactory;
  using intra_topology_factory_t = netuit::CompleteTopologyFactory;

  using program_t = sgpl::Program< sgpl_spec_t >;

  using service_manager_t = dish2::ServiceManager<
    dish2::DecayToBaselineService, // should run before cpu execution service
    dish2::RunningLogPurgeService, // should run before cpu execution service
    dish2::ControllerMappedStateNoiseService,

    dish2::InterpretedIntrospectiveStateRefreshService,

    dish2::IntermittentExtrospectiveStateExchangeService<this_t>,
    dish2::IntermittentExtrospectiveStateRotateService,
    dish2::IntermittentIntrospectiveStateExchangeService<this_t>,
    dish2::IntermittentIntrospectiveStateRotateService,
    dish2::CpuExecutionService,
    dish2::EventLaunchingService,
    dish2::IntermittentIntrospectiveStateRotateRestoreService,
    dish2::IntermittentIntrospectiveStateExchangeRestoreService,
    dish2::IntermittentExtrospectiveStateRotateRestoreService,
    dish2::IntermittentExtrospectiveStateExchangeRestoreService,

    dish2::IntermittentWritableStateExchangeService<this_t>,
    dish2::IntermittentWritableStateRotateService,

    dish2::BirthSetupService,
    dish2::CellAgeService,
    dish2::CollectiveHarvestingService,
    dish2::CollectiveResourceDecayService,
    dish2::ConduitFlushService,

    dish2::InterMessageLaunchingService,
    dish2::InterMessagePurgingService,
    dish2::IntraMessageLaunchingService,
    dish2::MessageCounterClearService,
    dish2::PushService,
    // dish2::QuorumCapService,
    dish2::QuorumService,
    dish2::ResourceDecayService,
    dish2::ResourceHarvestingService,
    dish2::ResourceReceivingService,
    dish2::ResourceSendingService,
    dish2::SpawnSendingService,
    dish2::StateInputJumpService,
    dish2::StateOutputPutService,

    dish2::EpochAdvanceService,

    dish2::IntermittentWritableStateRotateRestoreService,
    dish2::IntermittentWritableStateExchangeRestoreService,

    dish2::GroupExpirationService,
    // dish2::CellDeathService, // must run last
    dish2::ApoptosisService // must run last
  >;

  using thread_local_service_manager_t = dish2::ThreadLocalServiceManager<
    dish2::CellUpdateService,
    dish2::DiversityMaintenanceService,
    dish2::StintDiversityMaintenanceService
  >;

};


} // namespace dish2

#endif // #ifndef DISH2_SPEC_PREFAB_SPEC_DEFAULT_HPP_INCLUDE
