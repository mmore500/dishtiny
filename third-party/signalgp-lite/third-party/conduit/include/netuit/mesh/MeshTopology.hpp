#pragma once
#ifndef NETUIT_MESH_MESHTOPOLOGY_HPP_INCLUDE
#define NETUIT_MESH_MESHTOPOLOGY_HPP_INCLUDE

#include <map>
#include <set>
#include <stddef.h>

#include <mpi.h>

#include "../../uitsl/mpi/mpi_utils.hpp"
#include "../../uitsl/utility/assign_utils.hpp"

#include "../../uit/ducts/Duct.hpp"
#include "../../uit/fixtures/Conduit.hpp"

#include "../topology/Topology.hpp"

#include "MeshNode.hpp"

namespace netuit {
namespace internal {

template<typename ImplSpec>
class MeshTopology {

  using node_id_t = size_t;
  using edge_id_t = size_t;
  using node_t = MeshNode<ImplSpec>;
  using node_lookup_t = std::map<node_id_t, node_t>;

  // node_id -> node
  node_lookup_t nodes;

  // ordered by edge_id
  std::set<edge_id_t> edge_registry;
  // edge_id -> node_id
  std::map<edge_id_t, node_id_t> input_registry;
  std::map<edge_id_t, node_id_t> output_registry;

  void InitializeRegistries(const netuit::Topology& topology) {
    for (node_id_t node_id = 0; node_id < topology.GetSize(); ++node_id) {
      const netuit::TopoNode& topo_node = topology[node_id];
      RegisterNodeInputs(node_id, topo_node);
      RegisterNodeOutputs(node_id, topo_node);
    }
  }

  void RegisterNodeInputs(
    const node_id_t node_id, const netuit::TopoNode& topo_node
  ) {
    for (const netuit::TopoNodeInput & input : topo_node.GetInputs()) {
      emp_assert(input_registry.count(input.GetEdgeID()) == 0);
      edge_registry.insert(input.GetEdgeID());
      input_registry[input.GetEdgeID()] = node_id;
    }
  }

  void RegisterNodeOutputs(
    const node_id_t node_id, const netuit::TopoNode& topo_node
  ) {
    for (const netuit::TopoNodeOutput& output : topo_node.GetOutputs()) {
      emp_assert(output_registry.count(output.GetEdgeID()) == 0);
      edge_registry.insert(output.GetEdgeID());
      output_registry[output.GetEdgeID()] = node_id;
    }
  }

  void InitializeNodes(
    const netuit::Topology& topology,
    const std::function<uitsl::proc_id_t(node_id_t)> proc_assignment,
    const MPI_Comm& comm
  ) {
    // ensures that we include relevant nodes that don't have any edges
    for (node_id_t node_id = 0; node_id < topology.GetSize(); ++node_id) {
      if (proc_assignment(node_id) == uitsl::get_proc_id(comm)) {
        InitializeNode(node_id);
      }
    }

  }

  void InitializeNode(const node_id_t node_id){
    if (nodes.count(node_id) == 0) nodes.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(node_id),
      std::forward_as_tuple(node_id)
    );
  }

  void InitializeEdges(
    const netuit::Topology& topology,
    const std::function<uitsl::proc_id_t(node_id_t)> proc_assignment,
    const MPI_Comm& comm
  ) {

    std::map<edge_id_t, uit::Conduit<ImplSpec>> edge_conduits;

    // initialize inputs first...
    for (const edge_id_t edge : edge_registry) {
      const node_id_t input_id = input_registry.at(edge);
      const node_id_t output_id = output_registry.at(edge);
      // only construct infrastructure relevant to this proc
      // (but do need nodes that are connected to nodes on this proc)
      if (
        proc_assignment(input_id) == uitsl::get_proc_id(comm)
        || proc_assignment(output_id) == uitsl::get_proc_id(comm)
      ) {
        auto& conduit = edge_conduits[ edge ];

        InitializeNode(input_id);
        nodes.at(input_id).AddInput(
          MeshNodeInput<ImplSpec>{conduit.GetOutlet(), edge}
        );

      }
    }

    // then initialize outputs in reverse order
    // so that in cases where connections are reciporical
    // when a node's inputs and outputs zip together,
    // each (input, output) pair are associated with the same partner node
    for (
      auto it = std::rbegin(edge_registry); it != std::rend(edge_registry); ++it
    ) {
      const edge_id_t edge = *it;
      const node_id_t input_id = input_registry.at(edge);
      const node_id_t output_id = output_registry.at(edge);
      // only construct infrastructure relevant to this proc
      // (but do need nodes that are connected to nodes on this proc)
      if (
        proc_assignment(input_id) == uitsl::get_proc_id(comm)
        || proc_assignment(output_id) == uitsl::get_proc_id(comm)
      ) {
        auto& conduit = edge_conduits.at( edge );

        InitializeNode(output_id);
        nodes.at(output_id).AddOutput(
          MeshNodeOutput<ImplSpec>{conduit.GetInlet(), edge}
        );
      }
    }

  }

public:

  using value_type = typename node_lookup_t::value_type;

  MeshTopology(
    const netuit::Topology & topology,
    const std::function<uitsl::proc_id_t(node_id_t)> proc_assignment
      =uitsl::AssignIntegrated<uitsl::proc_id_t>{},
    const MPI_Comm comm=MPI_COMM_WORLD
  ) {
    InitializeRegistries(topology);
    InitializeNodes(topology, proc_assignment, comm);
    InitializeEdges(topology, proc_assignment, comm);

    // ensure that input, output registries have same keys as edge registry
    emp_assert(
      edge_registry == [this](){
        std::set<edge_id_t> res;
        std::transform(
          std::begin(input_registry),
          std::end(input_registry),
          std::inserter(res, std::begin(res)),
          [](const auto & kv){ return kv.first; }
        );
        return res;
      }()
    );
    emp_assert(
      edge_registry == [this](){
        std::set<edge_id_t> res;
        std::transform(
          std::begin(output_registry),
          std::end(output_registry),
          std::inserter(res, std::begin(res)),
          [](const auto & kv){ return kv.first; }
        );
        return res;
      }()
    );

  }

  size_t GetNodeCount() const { return nodes.size(); }

  size_t GetEdgeCount() const { return edge_registry.size(); }

  typename node_lookup_t::iterator begin() { return std::begin(nodes); }

  typename node_lookup_t::iterator end() { return std::end(nodes); }

  typename node_lookup_t::const_iterator begin() const {
    return std::begin(nodes);
  }

  typename node_lookup_t::const_iterator end() const {
    return std::end(nodes);
  }

  const std::set<edge_id_t>& GetEdgeRegistry() const { return edge_registry; }

  const std::map<edge_id_t, node_id_t>& GetInputRegistry() const {
    return input_registry;
  }

  const std::map<edge_id_t, node_id_t>& GetOutputRegistry() const {
    return output_registry;
  }


  std::string ToString() const {
    std::stringstream ss;
    ss << "nodes" << '\n';
    for ( const auto& [node_id, node] : nodes ) {
      ss << "node_id " << node_id << '\n';
      ss << "node " << node.ToString() << '\n';
    }

    // std::set<edge_id_t> edge_registry;

    // edge_id -> node_id
    ss << "input_registry " << '\n';
    for ( const auto& [edge_id, node_id] : input_registry ) {
      ss << edge_id << " -> " << node_id << '\n';
    }

    // edge_id -> node_id
    ss << "output_registry " << '\n';
    for ( const auto& [edge_id, node_id] : output_registry ) {
      ss << edge_id << " -> " << node_id << '\n';
    }

    return ss.str();
  }

};

} // namespace internal
} // namespace netuit

#endif // #ifndef NETUIT_MESH_MESHTOPOLOGY_HPP_INCLUDE
