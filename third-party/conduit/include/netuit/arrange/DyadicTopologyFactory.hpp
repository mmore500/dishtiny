#pragma once
#ifndef NETUIT_ARRANGE_DYADICTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_DYADICTOPOLOGYFACTORY_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

inline netuit::Topology make_dyadic_topology(const size_t cardinality) {

  netuit::Topology res;

  size_t edge_counter{};

  for (size_t dyad = 0; dyad < cardinality/2; ++dyad) {

      netuit::TopoEdge forward_edge{edge_counter++};
      netuit::TopoEdge backward_edge{edge_counter++};

      res.push_back(netuit::TopoNode{
        {backward_edge.GetOutlet()},
        {forward_edge.GetInlet()}
      });
      res.push_back(netuit::TopoNode{
        {forward_edge.GetOutlet()},
        {backward_edge.GetInlet()}
      });

  }

  // for odd cardinality, add a loop pipe
  if (cardinality%2) {
    netuit::TopoEdge self_edge{edge_counter++};

    res.push_back(netuit::TopoNode{
      {self_edge.GetOutlet()},
      {self_edge.GetInlet()}
    });
  };

  return res;

}

struct DyadicTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    return make_dyadic_topology(cardinality);
  }

  netuit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_dyadic_topology(cardinality.front());
  }

  static std::string GetName() { return "Dyadic Topology"; }

  static std::string GetSlug() { return "dyadic"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_DYADICTOPOLOGYFACTORY_HPP_INCLUDE
