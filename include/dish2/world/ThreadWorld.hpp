#pragma once
#ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
#define DISH2_WORLD_THREADWORLD_HPP_INCLUDE

#include <set>

#include "../../../third-party/conduit/include/netuit/mesh/Mesh.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../cell/cardinal_iterators/PushNodeOutputWrapper.hpp"
#include "../cell/Cell.hpp"
#include "../config/cfg.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/LogScope.hpp"
#include "../debug/PopulationExtinctionException.hpp"
#include "../introspection/make_causes_of_death_string_histogram.hpp"
#include "../push/DistanceToGraphCenterCellState.hpp"
#include "../push/DistanceToGraphCenterMessage.hpp"

namespace dish2 {

template<typename Spec>
struct ThreadWorld {

  using spec_t = Spec;

  using this_t = ThreadWorld<Spec>;

  using population_t = emp::vector< dish2::Cell<Spec> >;
  population_t population;

  using genome_mesh_spec_t = typename Spec::genome_mesh_spec_t;
  using genome_submesh_t
    = typename netuit::Mesh<genome_mesh_spec_t>::submesh_t;
  using genome_node_t = netuit::MeshNode<genome_mesh_spec_t>;

  using message_mesh_spec_t = typename Spec::message_mesh_spec_t;
  using message_submesh_t
    = typename netuit::Mesh<message_mesh_spec_t>::submesh_t;
  using message_node_t = netuit::MeshNode<message_mesh_spec_t>;

  using push_mesh_spec_t = typename Spec::push_mesh_spec_t;
  using push_submesh_t
    = typename netuit::Mesh<push_mesh_spec_t>::submesh_t;
  using push_node_t = netuit::MeshNode<push_mesh_spec_t>;

  using quorum_mesh_spec_t = typename Spec::quorum_mesh_spec_t;
  using quorum_submesh_t
    = typename netuit::Mesh<quorum_mesh_spec_t>::submesh_t;
  using quorum_node_t = netuit::MeshNode<quorum_mesh_spec_t>;

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
    const push_submesh_t& push_submesh,
    const quorum_submesh_t& quorum_submesh,
    const resource_submesh_t& resource_submesh,
    const state_submesh_t& state_submesh
  ) {

    emp_assert(( 1 == std::set<size_t>{
      genome_submesh.size(),
      message_submesh.size(),
      push_submesh.size(),
      quorum_submesh.size(),
      resource_submesh.size(),
      state_submesh.size()
    }.size() ));

    for (size_t i{}; i < message_submesh.size(); ++i) population.emplace_back(
      genome_submesh[i],
      message_submesh[i],
      push_submesh[i],
      quorum_submesh[i],
      resource_submesh[i],
      state_submesh[i]
    );

    // kick off distributed distance to center computation
    if ( uitsl::is_root() && dish2::thread_idx == 0 && population.size() ) {
      auto& target = population.front();

      auto& center_dist = std::get<dish2::DistanceToGraphCenterCellState>(
        target.cell_push_state
      );
      center_dist.my_distance = 0;

      const dish2::DistanceToGraphCenterMessage message{ 0 };
      std::for_each(
        target.template begin< dish2::PushNodeOutputWrapper<spec_t> >(),
        target.template end< dish2::PushNodeOutputWrapper<spec_t> >(),
        [&message]( auto& output ){ output.Put( message ); }
      );
    }

  }

  size_t update{};

  template<bool THROW_ON_EXTINCTION=true>
  void Update() {

    using thread_local_service_manager_t
      = typename Spec::thread_local_service_manager_t;

    thread_local_service_manager_t::template Run<this_t>( *this, update );

    if constexpr ( THROW_ON_EXTINCTION ) {
      if ( dish2::cfg.THROW_ON_EXTINCTION() && std::none_of(
        std::begin( population ), std::end( population ),
        []( const auto& cell ){ return cell.IsAlive(); }
      ) ) throw dish2::PopulationExtinctionException{
        update,
        population.size(),
        dish2::make_causes_of_death_string_histogram< ThreadWorld >( *this )
      };
    }

    ++update;

  }

  size_t GetUpdate() const { return update; }

  size_t GetSize() const { return population.size(); }

  const dish2::Cell<Spec>& GetCell(const size_t idx) const {
    return population[idx];
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
