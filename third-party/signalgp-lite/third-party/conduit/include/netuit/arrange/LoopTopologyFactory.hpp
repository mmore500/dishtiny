#pragma once
#ifndef NETUIT_ARRANGE_LOOPTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_LOOPTOPOLOGYFACTORY_HPP_INCLUDE

#include <algorithm>

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

inline netuit::Topology make_loop_topology(const size_t cardinality) {

  netuit::Topology res;

  size_t edge_counter{};

  std::generate_n(
    std::back_inserter(res),
    cardinality,
    [&](){
      netuit::TopoEdge self_edge{edge_counter++};
      return netuit::TopoNode{
        {self_edge.GetOutlet()},
        {self_edge.GetInlet()}
      };
    }
  );

  return res;

}

struct LoopTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    return make_loop_topology(cardinality);
  }

  netuit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_loop_topology(cardinality.front());
  }

  static std::string GetName() { return "Loop Topology"; }

  static std::string GetSlug() { return "loop"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_LOOPTOPOLOGYFACTORY_HPP_INCLUDE
