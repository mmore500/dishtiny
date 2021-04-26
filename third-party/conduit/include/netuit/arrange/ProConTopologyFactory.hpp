#pragma once
#ifndef NETUIT_ARRANGE_PROCONTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_PROCONTOPOLOGYFACTORY_HPP_INCLUDE

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

inline Topology make_producer_consumer_topology(const size_t cardinality) {

  Topology res;

  size_t edge_counter{};

  for (size_t pair = 0; pair < cardinality/2; ++pair) {

    netuit::TopoEdge edge{edge_counter++};
    res.push_back(TopoNode{
      {},
      {edge.GetInlet()}
    });
    res.push_back(TopoNode{
      {edge.GetOutlet()},
      {}
    });

  }

  // for odd cardinality, add a loop pipe
  if (cardinality%2) {

    netuit::TopoEdge self_edge{edge_counter++};
    res.push_back(TopoNode{
      {self_edge.GetOutlet()},
      {self_edge.GetInlet()}
    });

  };

  return res;

}

struct ProConTopologyFactory {

  Topology operator()(const size_t cardinality) const {
    return make_producer_consumer_topology(cardinality);
  }

  netuit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_producer_consumer_topology(cardinality.front());
  }

  static std::string GetName() { return "Producer-Consumer Topology"; }

  static std::string GetSlug() { return "procon"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_PROCONTOPOLOGYFACTORY_HPP_INCLUDE
