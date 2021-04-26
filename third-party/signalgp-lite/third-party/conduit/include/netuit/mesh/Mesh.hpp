#pragma once
#ifndef NETUIT_MESH_MESH_HPP_INCLUDE
#define NETUIT_MESH_MESH_HPP_INCLUDE

#include <ratio>
#include <stddef.h>
#include <unordered_map>

#include <mpi.h>

#include "../../uitsl/debug/safe_cast.hpp"
#include "../../uitsl/math/math_utils.hpp"
#include "../../uitsl/mpi/mpi_utils.hpp"
#include "../../uitsl/utility/assign_utils.hpp"

#include "../../uit/ducts/Duct.hpp"
#include "../../uit/setup/InterProcAddress.hpp"

#include "../assign/AssignIntegrated.hpp"
#include "../topology/Topology.hpp"

#include "MeshNode.hpp"
#include "MeshTopology.hpp"

namespace netuit {

namespace internal {

class MeshIDCounter {

  static inline size_t counter{};

public:

  static size_t Generate() { return counter++; }
  static void Reset() { counter = 0; }
  static size_t Get() { return counter; }


};

} // namespace internal

template<typename ImplSpec>
class Mesh {

  using node_id_t = size_t;
  using edge_id_t = size_t;
  using node_t = MeshNode<ImplSpec>;

  size_t mesh_id;
  MPI_Comm comm;

  // node_id -> node
  internal::MeshTopology<ImplSpec> nodes;

  std::function<uitsl::thread_id_t(node_id_t)> thread_assignment;
  std::function<uitsl::proc_id_t(node_id_t)> proc_assignment;

  using back_end_t = typename ImplSpec::ProcBackEnd;
  std::shared_ptr<back_end_t> back_end;

  void InitializeInterThreadDucts() {
    for (auto& [node_id, node] : nodes) {
      InitializeInterThreadDucts(node_id, node);
    }
  }

  void InitializeInterThreadDucts(const node_id_t node_id, node_t & node) {
    // only need to iterate through inputs because this fixes outputs' ducts too
    for (auto& input : node.GetInputs()) InitializeInterThreadDuct(input);
  }

  void InitializeInterThreadDuct(netuit::MeshNodeInput<ImplSpec> & input) {

    const node_id_t inlet_node_id = nodes.GetOutputRegistry().at(
      input.GetEdgeID()
    );
    const uitsl::thread_id_t inlet_thread = thread_assignment(inlet_node_id);

    const node_id_t outlet_node_id = nodes.GetInputRegistry().at(
      input.GetEdgeID()
    );
    const uitsl::thread_id_t outlet_thread = thread_assignment(outlet_node_id);

    if (inlet_thread != outlet_thread) input.template EmplaceDuct<
      typename ImplSpec::ThreadDuct
    >();

  }

  void InitializeInterProcDucts() {
    for (auto& [node_id, node] : nodes) {
      InitializeInterProcDucts(node_id, node);
    }
  }

  void InitializeInterProcDucts(const node_id_t node_id, node_t& node) {

    for (auto & input : node.GetInputs()) InitializeInterProcDuct(input);

    for (auto & output : node.GetOutputs()) InitializeInterProcDuct(output);

  }

  void InitializeInterProcDuct(netuit::MeshNodeInput<ImplSpec>& input) {
    const node_id_t inlet_node_id = nodes.GetOutputRegistry().at(
      input.GetEdgeID()
    );
    const uitsl::proc_id_t inlet_proc_id = proc_assignment(inlet_node_id);

    const node_id_t outlet_node_id = nodes.GetInputRegistry().at(
      input.GetEdgeID()
    );
    const uitsl::proc_id_t outlet_proc_id = proc_assignment(outlet_node_id);

    static std::unordered_set<int> tag_checker;
    const int tag = uitsl::safe_cast<int>(
      uitsl::sidebyside_hash<std::ratio<3, 4>>(mesh_id, input.GetEdgeID())
    );

    const uit::InterProcAddress addr{
      outlet_proc_id,
      inlet_proc_id,
      thread_assignment(outlet_node_id),
      thread_assignment(inlet_node_id),
      tag,
      comm
    };

    if (inlet_proc_id != outlet_proc_id) {
      input.template SplitDuct<
        typename ImplSpec::ProcOutletDuct
      >(addr, back_end);
      // assert that generated tags are unique
      emp_assert( tag_checker.insert(tag).second );
    }

  }

  void InitializeInterProcDuct(netuit::MeshNodeOutput<ImplSpec>& output) {
    const node_id_t inlet_node_id = nodes.GetOutputRegistry().at(
      output.GetEdgeID()
    );
    const uitsl::proc_id_t inlet_proc_id = proc_assignment(inlet_node_id);

    const node_id_t outlet_node_id = nodes.GetInputRegistry().at(
      output.GetEdgeID()
    );
    const uitsl::proc_id_t outlet_proc_id = proc_assignment(outlet_node_id);

    const uit::InterProcAddress addr{
      outlet_proc_id,
      inlet_proc_id,
      thread_assignment(outlet_node_id),
      thread_assignment(inlet_node_id),
      uitsl::safe_cast<int>(
        uitsl::sidebyside_hash<std::ratio<3, 4>>(mesh_id, output.GetEdgeID())
      ),
      comm
    };

    if (inlet_proc_id != outlet_proc_id) output.template SplitDuct<
      typename ImplSpec::ProcInletDuct
    >(addr, back_end);


  }


public:

  Mesh(
    const Topology & topology,
    const std::function<uitsl::thread_id_t(node_id_t)> thread_assignment_
      =uitsl::AssignIntegrated<uitsl::thread_id_t>{},
    const std::function<uitsl::proc_id_t(node_id_t)> proc_assignment_
      =uitsl::AssignIntegrated<uitsl::proc_id_t>{},
    std::shared_ptr<back_end_t> back_end_=std::make_shared<back_end_t>(),
    const MPI_Comm comm_=MPI_COMM_WORLD,
    const size_t mesh_id_=internal::MeshIDCounter::Generate()
  )
  : mesh_id(mesh_id_)
  , comm(comm_)
  , nodes(topology, proc_assignment_, comm)
  , thread_assignment(thread_assignment_)
  , proc_assignment(proc_assignment_)
  , back_end(back_end_) {
    InitializeInterThreadDucts();
    InitializeInterProcDucts();
    back_end->Initialize();
  }

  // TODO rename GetNumNodes
  size_t GetNodeCount() const { return nodes.GetNodeCount(); }

  // TODO rename GetNumEdges
  size_t GetEdgeCount() const { return nodes.GetEdgeCount(); }

  using submesh_t = emp::vector<node_t>;

  submesh_t GetSubmesh(const uitsl::thread_id_t tid=0) const {
    return GetSubmesh(tid, uitsl::get_proc_id(comm));
  }

  submesh_t GetSubmesh(
    const uitsl::thread_id_t tid,
    const uitsl::proc_id_t pid
  ) const {
    submesh_t res;
    for (const auto& [node_id, node] : nodes) {
      if (
        thread_assignment(node_id) == tid
        && proc_assignment(node_id) == pid
      ) res.push_back(node);
    }
    return res;
  }

  std::string ToString() const {
    std::stringstream ss;
    for (const auto& [node_id, node] : nodes) {
      ss << uitsl::format_member(
        "node id", node_id
      );
      ss << uitsl::format_member(
        "proc assignment", proc_assignment(node_id)
      );
      ss << uitsl::format_member(
        "thread assignment", thread_assignment(node_id)
      );
      ss << uitsl::format_member(
        "node", node.ToString()
      );
      ss << '\n';
    }
    return ss.str();
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_MESH_MESH_HPP_INCLUDE
