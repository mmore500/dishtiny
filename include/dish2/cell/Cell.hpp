#pragma once
#ifndef DISH2_CELL_CELL_HPP_INCLUDE
#define DISH2_CELL_CELL_HPP_INCLUDE

#include <utility>

#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/Empirical/source/base/optional.h"
#include "../../../third-party/Empirical/source/base/vector.h"

#include "../config/cfg.hpp"
#include "../genome/Genome.hpp"
#include "../services/_index.hpp"
#include "../services/ServiceManager.hpp"

#include "Cardinal.hpp"

#include "cardinal_iterators/IdentityWrapper.hpp"

namespace dish2 {

template<typename Spec>
struct Cell {

  emp::vector< dish2::Cardinal<Spec> > cardinals;

  using genome_t = dish2::Genome<Spec>;
  emp::optional< genome_t > genome{ std::in_place, std::in_place };

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_node_t = netuit::MeshNode<genome_mesh_spec_t>;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_t = netuit::MeshNode<message_mesh_spec_t>;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_node_t = netuit::MeshNode<resource_mesh_spec_t>;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_node_t = netuit::MeshNode<state_mesh_spec_t>;

  using this_t = dish2::Cell<Spec>;

  // out of class implementations
  void DeathRoutine();
  void HeirPayoutRoutine();
  void MakeAliveRoutine();

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
    cardinals.reserve( message_node.GetNumInputs() );
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

    MakeAliveRoutine();

  }

  void Clear() {
    genome.reset();
    for (auto& cardinal : cardinals) cardinal.Reset();
  }

  template<typename T=dish2::IdentityWrapper<Spec>>
  T begin() { return T{ cardinals.begin() }; }

  // TODO fix bad const_cast
  template<typename T=dish2::IdentityWrapper<Spec>>
  T begin() const { return T{ const_cast<Cell*>(this)->cardinals.begin() }; }

  template<typename T=dish2::IdentityWrapper<Spec>>
  T end() { return T{ cardinals.end() }; }

  // TODO fix bad const_cast
  template<typename T=dish2::IdentityWrapper<Spec>>
  T end() const { return T{ const_cast<Cell*>(this)->cardinals.end() }; }

  bool IsAlive() const {
    emp_assert( cardinals.front().peripheral.readable_state.template Get<
      dish2::IsAlive
    >().Get() == genome.has_value() );
    return genome.has_value();
  }

  size_t GetNumCardinals() const { return cardinals.size(); }

  const genome_t& GetGenome() const { return *genome; }

  void Update(const size_t update) {

    // TODO put these in order
    using service_manager_t = dish2::ServiceManager<
      dish2::BirthSetupService,
      dish2::ConduitFlushService,
      dish2::CpuExecutionService,
      dish2::EventLaunchingService,
      dish2::InterMessageLaunchingService,
      dish2::IntraMessageLaunchingService,
      dish2::ResourceDecayService,
      dish2::ResourceHarvestingService,
      dish2::ResourceSendingService,
      dish2::SpawnSendingService,
      dish2::StateInputJumpService,
      dish2::ApoptosisService // must run last
    >;

    service_manager_t::template Run<this_t>( *this, update, IsAlive() );

  }

};

} // namespace dish2

// include out of class implementtions
#include "routine_impls/_index.hpp"

#endif // #ifndef DISH2_CELL_CELL_HPP_INCLUDE
