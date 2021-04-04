#pragma once
#ifndef DISH2_CELL_CELL_HPP_INCLUDE
#define DISH2_CELL_CELL_HPP_INCLUDE

#include <utility>

#include "../../../third-party/conduit/include/netuit/assign/AssignIntegrated.hpp"
#include "../../../third-party/conduit/include/netuit/assign/AssignThisProc.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/Mesh.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../config/cfg.hpp"
#include "../enum/CauseOfDeath.hpp"
#include "../events/EventManager.hpp"
#include "../events/_index.hpp"
#include "../genome/Genome.hpp"
#include "../push/PushCellState.hpp"
#include "../quorum/CellQuorumState.hpp"
#include "../runninglog/RunningLogs.hpp"

#include "Cardinal.hpp"

#include "cardinal_iterators/IdentityWrapper.hpp"
#include "cardinal_iterators/NeighborKinGroupIDViewWrapper.hpp"
#include "cardinal_iterators/ResourceStockpileWrapper.hpp"

namespace dish2 {

template<typename Spec>
struct Cell {

  emp::vector< dish2::Cardinal<Spec> > cardinals;

  dish2::RunningLogs<Spec> running_logs;

  using genome_t = dish2::Genome<Spec>;
  emp::optional< genome_t > genome{ std::in_place, std::in_place };

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_node_t = netuit::MeshNode<genome_mesh_spec_t>;

  using intra_message_mesh_spec_t = typename Spec::intra_message_mesh_spec_t;
  using intra_message_node_t = netuit::MeshNode<intra_message_mesh_spec_t>;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_node_t = netuit::MeshNode<message_mesh_spec_t>;

  using push_mesh_spec_t = typename Spec::push_mesh_spec_t;
  using push_node_t = netuit::MeshNode<push_mesh_spec_t>;

  dish2::PushCellState cell_push_state;

  using quorum_mesh_spec_t = typename Spec::quorum_mesh_spec_t;
  using quorum_node_t = netuit::MeshNode<quorum_mesh_spec_t>;

  dish2::CellQuorumState< Spec > cell_quorum_state;

  using resource_mesh_spec_t = typename Spec::resource_mesh_spec_t;
  using resource_node_t = netuit::MeshNode<resource_mesh_spec_t>;

  using state_mesh_spec_t = typename Spec::state_mesh_spec_t;
  using state_node_t = netuit::MeshNode<state_mesh_spec_t>;

  using spec_t = Spec;
  using this_t = dish2::Cell<Spec>;

  // out of class implementations
  void DeathRoutine(const dish2::CauseOfDeath);
  void FragmentationRoutine();
  void HeirPayoutRoutine();
  void MakeAliveRoutine();

  Cell(
    const genome_node_t& genome_node,
    const message_node_t& message_node,
    const push_node_t& push_node,
    const quorum_node_t& quorum_node,
    const resource_node_t& resource_node,
    const state_node_t& state_node
  ) {

    // make sure that input and output counts are consistent
    emp_assert(( 1 == std::set<size_t>{
      genome_node.GetNumInputs(),
      message_node.GetNumInputs(),
      push_node.GetNumInputs(),
      quorum_node.GetNumInputs(),
      resource_node.GetNumInputs(),
      state_node.GetNumInputs(),
      genome_node.GetNumOutputs(),
      message_node.GetNumOutputs(),
      push_node.GetNumOutputs(),
      quorum_node.GetNumOutputs(),
      resource_node.GetNumOutputs(),
      state_node.GetNumOutputs(),
    }.size() ));

    const size_t num_cardinals = message_node.GetNumInputs();

    using intra_message_mesh_t = netuit::Mesh<intra_message_mesh_spec_t>;
    using intra_message_backend_t
      = typename intra_message_mesh_spec_t::ProcBackEnd;
    using intra_message_mesh_topology_factory_t
      = typename Spec::intra_topology_factory_t;
    intra_message_mesh_t intra_message_mesh{
      intra_message_mesh_topology_factory_t{}( num_cardinals ),
      uitsl::AssignIntegrated<uitsl::thread_id_t>{},
      netuit::AssignThisProc{},
      std::make_shared<intra_message_backend_t>(),
      MPI_COMM_WORLD,
      0 // const size_t mesh_id_
    };

    auto intra_message_submesh = intra_message_mesh.GetSubmesh(0);

    emp_assert( intra_message_submesh.size() == num_cardinals );

    // set up cardinals, one for each cell neighbor
    cardinals.reserve( num_cardinals );
    for (size_t i{}; i < num_cardinals; ++i) {
      cardinals.emplace_back(
        genome_node.GetInput(i),
        genome_node.GetOutput(i),
        message_node.GetInput(i),
        message_node.GetOutput(i),
        push_node.GetInput(i),
        push_node.GetOutput(i),
        quorum_node.GetInput(i),
        quorum_node.GetOutput(i),
        resource_node.GetInput(i),
        resource_node.GetOutput(i),
        state_node.GetInput(i),
        state_node.GetOutput(i),
        intra_message_submesh[i]
      );
    }

    MakeAliveRoutine();

    // setup start resource
    std::fill(
      this->template begin< dish2::ResourceStockpileWrapper<Spec> >(),
      this->template end< dish2::ResourceStockpileWrapper<Spec> >(),
      sgpl::tlrand.Get().GetDouble(
        dish2::cfg.MIN_START_RESOURCE(), dish2::cfg.MAX_START_RESOURCE()
      )
    );

  }

  void Clear() {
    genome.reset();
    for (auto& cardinal : cardinals) cardinal.Reset();
  }

  template<
    typename T=dish2::IdentityWrapper<Spec>, typename... Args
  >
  T begin( Args&&... args ) {
    return T{ cardinals.begin(), std::forward<Args>(args)... };
  }

  // TODO fix bad const_cast
  template<
    typename T=dish2::IdentityWrapper<Spec>, typename... Args
  >
  T begin( Args&&... args ) const {
    return T{
      const_cast<Cell*>(this)->cardinals.begin(),
    };
  }

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

  size_t GetPeripherality( const size_t lev ) const {
    if ( !IsAlive() ) return false;

    const auto kin_group_id = genome->kin_group_id.GetBuffer()[ lev ];
    return std::count_if(
      this->begin<dish2::NeighborKinGroupIDViewWrapper<Spec>>(),
      this->end<dish2::NeighborKinGroupIDViewWrapper<Spec>>(),
      [lev, kin_group_id]( const auto& neighbor_kin_group_id_view ){
        return kin_group_id != neighbor_kin_group_id_view.Get( lev );
      }
    );
  }

  auto GetPeripherality() const {
    emp::array<size_t, Spec::NLEV> res;
    for (size_t l{}; l < Spec::NLEV; ++l) res[l] = GetPeripherality(l);
    return res;
  }

  size_t GetNumCardinals() const { return cardinals.size(); }

  const genome_t& GetGenome() const { return *genome; }

  void Update(const size_t update) {

    using service_manager_t = typename Spec::service_manager_t;

    service_manager_t::template Run<this_t>( *this, update, IsAlive() );

  }

};

} // namespace dish2

// include out of class implementtions
#include "routine_impls/_index.hpp"

#endif // #ifndef DISH2_CELL_CELL_HPP_INCLUDE
