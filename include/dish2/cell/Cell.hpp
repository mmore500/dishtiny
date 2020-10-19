#pragma once
#ifndef DISH2_CELL_CELL_HPP_INCLUDE
#define DISH2_CELL_CELL_HPP_INCLUDE

#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/Empirical/source/base/optional.h"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "../config/cfg.hpp"
#include "../genome/Genome.hpp"

#include "Cardinal.hpp"

#include "cardinal_iterators/IdentityWrapper.hpp"
#include "should_runs/_index.hpp"

namespace dish2 {

template<typename Spec>
class Cell {

  emp::vector< dish2::Cardinal<Spec> > cardinals;

  emp::optional< dish2::Genome<Spec> > genome{ dish2::cfg.PROGRAM_START_SIZE()};

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_node_t = netuit::MeshNode<genome_mesh_spec_t>;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_t = netuit::MeshNode<message_mesh_spec_t>;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_node_t = netuit::MeshNode<resource_mesh_spec_t>;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_node_t = netuit::MeshNode<state_mesh_spec_t>;

protected:

  // out of class implementations
  void DeathRoutine();
  void HeirPayoutRoutine();
  void MakeAliveroutine();

  // out of class implementations
  void ApoptosisService();
  void BirthSetupService();
  void ConduitFlushService();
  void CpuExecutionService();
  void EventLaunchingService();
  void InterMessageLaunchingService();
  void IntraMessageLaunchingService();
  void ResourceDecayService();
  void ResourceHarvestingService();
  void ResourceReceivingService();
  void ResourceSendingService();
  void SpawnSendingService();
  void StateInputJumpService();

public:

  Cell(
    const genome_node_t& genome_node,
    const message_node_t& message_node,
    const resource_node_t& resource_node,
    const state_node_t& state_node
  ) {

    // make sure that input and output counts are consistent
    emp_assert(( 1 == std::set<size_t>{
      genome_node.GetNumInputs(),
      message_node.GetNumInputs(),
      resource_node.GetNumInputs(),
      state_node.GetNumInputs(),
      genome_node.GetNumOutputs(),
      message_node.GetNumOutputs(),
      resource_node.GetNumOutputs(),
      state_node.GetNumOutputs(),
    }.size() ));

    // set up cardinals, one for each cell neighbor
    for (size_t i{}; i < message_node.GetNumInputs(); ++i) {
      cardinals.emplace_back(
        genome_node.GetInput(i),
        genome_node.GetOutput(i),
        message_node.GetInput(i),
        message_node.GetOutput(i),
        resource_node.GetInput(i),
        resource_node.GetOutput(i),
        state_node.GetInput(i),
        state_node.GetOutput(i)
      );
    }

    MakeAliveroutine();

  }

  void Clear() {
    genome.reset();
    for (auto& cardinal : cardinals) cardinal.Reset();
  }

  template<typename T=dish2::IdentityWrapper<Spec>>
  T begin() { return T{ cardinals.begin() }; }

  template<typename T=dish2::IdentityWrapper<Spec>>
  T end() { return T{ cardinals.end() }; }

  bool IsAlive() const {
    return cardinals.front().peripheral.readable_state.template Get<
      dish2::IsAlive
    >().Get();
  }

  void Update(const size_t update) {
    const bool is_alive{ IsAlive() };

    // TODO put these in order
    if ( dish2::apoptosis_should_run( update, is_alive ) ) {
      ApoptosisService();
    }

    if ( dish2::birth_setup_should_run( update, is_alive ) ) {
      BirthSetupService();
    }

    if ( dish2::conduit_flush_should_run( update, is_alive ) ) {
      ConduitFlushService();
    }

    if ( dish2::cpu_execution_should_run( update, is_alive ) ) {
      CpuExecutionService();
    }

    if ( dish2::event_launching_should_run( update, is_alive ) ) {
      EventLaunchingService();
    }

    if ( dish2::inter_message_launching_should_run( update, is_alive ) ) {
      InterMessageLaunchingService();
    }

    if ( dish2::intra_message_launching_should_run( update, is_alive ) ) {
      IntraMessageLaunchingService();
    }

    if ( dish2::resource_decay_should_run( update, is_alive ) ) {
      ResourceDecayService();
    }

    if ( dish2::resource_harvesting_should_run( update, is_alive ) ) {
      ResourceHarvestingService();
    }

    if ( dish2::resource_receiving_should_run( update, is_alive ) ) {
      ResourceReceivingService();
    }

    if ( dish2::resource_sending_should_run( update, is_alive ) ) {
      ResourceSendingService();
    }

    if ( dish2::spawn_sending_should_run( update, is_alive ) ) {
      SpawnSendingService();
    }

    if ( dish2::spawn_sending_should_run( update, is_alive ) ) {
      StateInputJumpService();
    }

  }

};

} // namespace dish2

// include out of class implementtions
#include "routine_impls/_index.hpp"
#include "service_impls/_index.hpp"

#endif // #ifndef DISH2_CELL_CELL_HPP_INCLUDE
