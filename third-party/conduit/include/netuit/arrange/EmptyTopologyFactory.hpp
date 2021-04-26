#pragma once
#ifndef NETUIT_ARRANGE_EMPTYTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_EMPTYTOPOLOGYFACTORY_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {

netuit::Topology make_empty_topology(const size_t cardinality) {

  emp::vector<netuit::TopoNode> res( cardinality );

  return netuit::Topology{ res };

}

struct EmptyTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    return make_empty_topology(cardinality);
  }

  netuit::Topology operator()(const emp::vector<size_t> cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_empty_topology(cardinality.front());
  }
  static std::string GetName() { return "Empty Topology"; }

  static std::string GetSlug() { return "empty"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_EMPTYTOPOLOGYFACTORY_HPP_INCLUDE
