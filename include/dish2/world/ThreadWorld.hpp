#pragma once
#ifndef DISH2_WORLD_THREADWORLD_HPP_INCLUDE
#define DISH2_WORLD_THREADWORLD_HPP_INCLUDE

#include <set>

#include "../../../third-party/conduit/include/netuit/mesh/Mesh.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNode.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/Cell.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../utility/PopulationExtinctionException.hpp"

namespace dish2 {

template<typename Spec>
struct ThreadWorld {

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
    const quorum_submesh_t& quorum_submesh,
    const resource_submesh_t& resource_submesh,
    const state_submesh_t& state_submesh
  ) {

    emp_assert(( 1 == std::set<size_t>{
      genome_submesh.size(),
      message_submesh.size(),
      quorum_submesh.size(),
      resource_submesh.size(),
      state_submesh.size()
    }.size() ));

    for (size_t i{}; i < message_submesh.size(); ++i) population.emplace_back(
      genome_submesh[i],
      message_submesh[i],
      quorum_submesh[i],
      resource_submesh[i],
      state_submesh[i]
    );

  }

  size_t update{};

  template<bool THROW_ON_EXTINCTION=true>
  void Update() {
    uitsl::for_each(
      std::begin( population ), std::end( population ),
      sgpl::CountingIterator{},
      [this](auto& cell, const size_t i){
        const dish2::LogScope guard{
          emp::to_string("updating cell ", i),
          "We're having the nth cell run its program and interact with the environment. All cells will take a turn at this one-by-one.",
          3
        };
        cell.Update(update);
      }
    );

    if constexpr ( THROW_ON_EXTINCTION ) {
      if ( dish2::cfg.THROW_ON_EXTINCTION() && std::none_of(
        std::begin( population ), std::end( population ),
        []( const auto& cell ){ return cell.IsAlive(); }
      ) ) throw dish2::PopulationExtinctionException{
        update,
        population.size()
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
